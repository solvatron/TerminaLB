#include "lattice.hpp"
#include <vector>
#include <cmath>

void Lattice::initialize(){
    //initializeVelocityF();
    //initializeDensityF();
    initializeDistributionF();
}

void Lattice::initializeDistributionF(){
    distributionF.resize(parameters.getDomainX() * parameters.getDomainY() * numDirections, 0.0f);
}

void Lattice::collide(){
    std::vector<float> distributions(9, 0);
    std::vector<float> eqDistributions(9, 0);
    int density;
    std::vector<float> velocity(2, 0);
    std::vector<int> latticeCoords(2, 0);

    for(int cellX = 0; cellX < parameters.getDomainX(); cellX++){
        for(int cellY = 0; cellY < parameters.getDomainY(); cellY++){
            latticeCoords = {cellX, cellY};
            getDistributions(latticeCoords, distributions);
            getRhoUfromDistributions(distributions, density, velocity);
            getEqDistribution(density, velocity, eqDistributions);

            relaxDistributions(eqDistributions, distributions);
            setDistributions(latticeCoords, distributions);
        }
    }

    incrementDisplacement();
}

void Lattice::getDistributions(const std::vector<int>& latticeCoords, std::vector<float>& distributions){
    for(int direction = 0; direction < numDirections; direction++){
        int memoryX = (latticeCoords[0] + displacementVectors[direction][0]) % parameters.getDomainX();
        int memoryY = (latticeCoords[1] + displacementVectors[direction][1]) % parameters.getDomainY();
        int memoryIndex = direction + (numDirections * memoryX) + (parameters.getDomainX() * numDirections * memoryY);

        distributions[direction] = distributionF[memoryIndex];
    }
}

void Lattice::setDistributions(const std::vector<int>& latticeCoords, const std::vector<float>& distributions){
    for(int direction = 0; direction < numDirections; direction++){
        int memoryX = (latticeCoords[0] + displacementVectors[direction][0]) % parameters.getDomainX();
        int memoryY = (latticeCoords[1] + displacementVectors[direction][1]) % parameters.getDomainY();
        int memoryIndex = direction + (numDirections * memoryX) + (parameters.getDomainX() * numDirections * memoryY);

        distributionF[memoryIndex] = distributions[direction];
    }
}

void Lattice::getRhoUfromDistributions(const std::vector<float>& distributions, int density, std::vector<float>& velocity){
    density = 0;
    for(int direction; direction < numDirections; direction++){
        density += distributions[direction];
    }

    std::vector<float> moment_1(2, 0);
    for(int direction; direction < numDirections; direction++){
        moment_1[0] += latticeC[direction][0] * distributions[direction];
        moment_1[1] += latticeC[direction][1] * distributions[direction];
    }
    velocity[0] = moment_1[0] / density;
    velocity[1] = moment_1[1] / density;
}

void Lattice::getEqDistribution(const float density, const std::vector<float>& velocity, std::vector<float>& eqDistributions){
    for(int direction = 0; direction < numDirections; direction++){
        eqDistributions[direction] = weights[direction] * density * (
                                        1. + (latticeC[direction][0] * velocity[0] + latticeC[direction][1] * velocity[1]) / pow(c_s, 2)
                                + 1./2. * pow(latticeC[direction][0] * velocity[0] + latticeC[direction][1] * velocity[1], 2) / pow(c_s, 4)
                                  - 1./2. *  (           velocity[0] * velocity[0] +            velocity[1] * velocity[1]) / pow(c_s, 2));
    }
}

void Lattice::relaxDistributions(const std::vector<float>& eqDistributions, std::vector<float>& distributions){
    float relaxationTime = parameters.getRelaxationTime();
    for(int direction; direction < numDirections; direction++){
        distributions[direction] = distributions[direction] - 1. / relaxationTime * (distributions[direction] - eqDistributions[direction]);
    }
}
