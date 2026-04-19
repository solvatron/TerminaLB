#include "lattice.hpp"
#include "../utils/utils.hpp"
#include <iostream>
#include <vector>
#include <cmath>

void Lattice::initialize(){
    initializeDistributionFTest();
    boundaryBuffer.initializeBoundaryBuffer(parameters.getDomainX(), parameters.getDomainY());
}

void Lattice::collide(){
    std::vector<float> distributions(9, 0);
    std::vector<float> eqDistributions(9, 0);
    float density;
    std::vector<float> velocity(2, 0);
    std::vector<int> latticeCoords(2, 0);

    for(int cellX = 0; cellX < parameters.getDomainX(); cellX++){
        for(int cellY = 0; cellY < parameters.getDomainY(); cellY++){
            //std::cout << "[Collide] Colliding cell (" << cellX << ", " << cellY << ")" << std::endl;
            latticeCoords = {cellX, cellY};
            getDistributions(latticeCoords, distributions);
            //std::cout << "[Collide] Got distributions" << std::endl;
            getRhoUfromDistributions(distributions, density, velocity);
            //std::cout << "[Collide] Got RhoU" << std::endl;
            getEqDistribution(density, velocity, eqDistributions);
            //std::cout << "[Collide] Got equlibrium distributions" << std::endl;

            relaxDistributions(eqDistributions, distributions);
            //std::cout << "[Collide] Relaxed distributions" << std::endl;
            setDistributions(latticeCoords, distributions);
            //std::cout << "[Collide] Set distributions" << std::endl;
        }
    }
}

void Lattice::stream(){
    boundaryBuffer.setBoundaryBuffer(*this);
    std::cout << "[Stream] Set Boundary Buffer" << std::endl;
    incrementDisplacement();
    std::cout << "[Stream] Incremented Displacement" << std::endl;
    for(int boundary = 1; boundary <= 4; boundary++){
        if(parameters.isInlet(boundary)){
            //boundaryBuffer.setInlet(boundary, *this, parameters.getInletVelocity(boundary));
        }

        if(parameters.isOutlet(boundary)){
            //boundaryBuffer.setOutlet(boundary, *this);
        }
    }
    boundaryBuffer.writeBoundaryBuffer(*this);
    std::cout << "[Stream] Wrote Boundary Buffer" << std::endl;

    step++;
}

void Lattice::initializeDistributionF(const std::vector<float>& densityF, const std::vector<float>& velocityF){
    distributionF.resize(parameters.getDomainX() * parameters.getDomainY() * numDirections, 0.0f);

    for(int cellX = 0; cellX < parameters.getDomainX(); cellX++){
        for(int cellY = 0; cellY < parameters.getDomainY(); cellY++){
            std::vector<int> latticeCoords = {cellX, cellY};
            std::vector<float> velocity(2, 0);
            std::vector<float> eqDistributions(9, 0);

            float density = densityF[cellY * parameters.getDomainX() + cellX];
            velocity[0]  = velocityF[(cellY * parameters.getDomainX() + cellX) * 2];
            velocity[1]  = velocityF[(cellY * parameters.getDomainX() + cellX) * 2 + 1];

            getEqDistribution(density, velocity, eqDistributions);
            setDistributions(latticeCoords, eqDistributions);
        }
    }
}

void Lattice::initializeDistributionF(){
    std::vector<float> densityF(parameters.getDomainX() * parameters.getDomainY(), 1.0f);
    std::vector<float> velocityF(parameters.getDomainX() * parameters.getDomainY() * 2, 0.0f);
    initializeDistributionF(densityF, velocityF);
}

void Lattice::initializeDistributionFTest(){
    std::vector<float> densityF(parameters.getDomainX() * parameters.getDomainY(), 1.0f);
    std::vector<float> velocityF(parameters.getDomainX() * parameters.getDomainY() * 2, 0.0f);
    for(int cellX = 0; cellX < parameters.getDomainX(); cellX++){
        for(int cellY = 0; cellY < parameters.getDomainY(); cellY++){
            if(cellX > parameters.getDomainX()/2){
                velocityF[(cellY * parameters.getDomainX() + cellX) * 2] = 0.01;
                //velocityF[(cellY * parameters.getDomainX() + cellX) * 2 + 1] = 0.01;
            }
        }
    }
    initializeDistributionF(densityF, velocityF);
}


void Lattice::getDistributions(const std::vector<int>& latticeCoords, std::vector<float>& distributions) const{
    for(int direction = 0; direction < numDirections; direction++){
        int memoryIndex = getMemoryIndex(latticeCoords, direction);
        distributions[direction] = distributionF[memoryIndex];
    }
}

void Lattice::setDistributions(const std::vector<int>& latticeCoords, const std::vector<float>& distributions){
    for(int direction = 0; direction < numDirections; direction++){
        int memoryIndex = getMemoryIndex(latticeCoords, direction);
        distributionF[memoryIndex] = distributions[direction];
    }
}

float Lattice::getSingleDistribution(const std::vector<int>& latticeCoords, const int direction) const{
    int memoryIndex = getMemoryIndex(latticeCoords, direction);
    return distributionF[memoryIndex];
}

void Lattice::setSingleDistribution(const std::vector<int>& latticeCoords, const int direction, const float distribution){
    int memoryIndex = getMemoryIndex(latticeCoords, direction);
    distributionF[memoryIndex] = distribution;
}

int Lattice::getMemoryIndex(const std::vector<int>& latticeCoords, const int direction) const{
    int memoryX = (latticeCoords[0] + displacementVectors[direction][0] + parameters.getDomainX()) % parameters.getDomainX();
    int memoryY = (latticeCoords[1] + displacementVectors[direction][1] + parameters.getDomainY()) % parameters.getDomainY();
    return direction + (numDirections * memoryX) + (parameters.getDomainX() * numDirections * memoryY);
}

void Lattice::getRhoUfromDistributions(const std::vector<float>& distributions, float& density, std::vector<float>& velocity) const{
    density = 0;
    for(int direction = 0; direction < numDirections; direction++){
        density += distributions[direction];
    }

    std::vector<float> moment_1(2, 0);
    for(int direction = 0; direction < numDirections; direction++){
        moment_1[0] += latticeC[direction][0] * distributions[direction];
        moment_1[1] += latticeC[direction][1] * distributions[direction];
    }
    velocity[0] = moment_1[0] / density;
    velocity[1] = moment_1[1] / density;
}

void Lattice::getEqDistribution(const float density, const std::vector<float>& velocity, std::vector<float>& eqDistributions){
    eqDistributions.resize(numDirections, 0);
    for(int direction = 0; direction < numDirections; direction++){
        eqDistributions[direction] = weights[direction] * density * (
                                        1. + (latticeC[direction][0] * velocity[0] + latticeC[direction][1] * velocity[1]) / pow(c_s, 2)
                                + 1./2. * pow(latticeC[direction][0] * velocity[0] + latticeC[direction][1] * velocity[1], 2) / pow(c_s, 4)
                                  - 1./2. *  (           velocity[0] * velocity[0] +            velocity[1] * velocity[1]) / pow(c_s, 2));
    }
}

void Lattice::relaxDistributions(const std::vector<float>& eqDistributions, std::vector<float>& distributions){
    float relaxationTime = parameters.getRelaxationTime();
    for(int direction = 0; direction < numDirections; direction++){
        distributions[direction] = distributions[direction] - 1. / relaxationTime * (distributions[direction] - eqDistributions[direction]);
    }
}

void Lattice::relaxCorners(){
    float density = 1;
    std::vector<float> velocity(2, 0);
    std::vector<float> eqDistributions(numDirections, 0);

    getEqDistribution(density, velocity, eqDistributions);
    setDistributions({0, 0}, eqDistributions);
    setDistributions({parameters.getDomainX()-1, 0}, eqDistributions);
    setDistributions({0, parameters.getDomainY()-1}, eqDistributions);
    setDistributions({parameters.getDomainX()-1, parameters.getDomainY()-1}, eqDistributions);
}


void Lattice::incrementDisplacement(){
    for(int direction = 0; direction < numDirections; direction++){
        std::vector<int> incrementVec = latticeC[utils::reverse(direction)];
        displacementVectors[direction][0] += incrementVec[0] + parameters.getDomainX();
        displacementVectors[direction][1] += incrementVec[1] + parameters.getDomainY();

        displacementVectors[direction][0] = displacementVectors[direction][0]%parameters.getDomainX();
        displacementVectors[direction][1] = displacementVectors[direction][1]%parameters.getDomainY();
    }
}

int Lattice::getTimeStep() const{
    return step;
}

void Lattice::getRhoUField(std::vector<float>& densityF, std::vector<float>& velocityF) const{
    densityF.resize(parameters.getDomainX() * parameters.getDomainY(), 0.0f);
    velocityF.resize(parameters.getDomainX() * parameters.getDomainY() * 2, 0.0f);

    std::vector<float> distributions(9, 0);
    for(int cellX = 0; cellX < parameters.getDomainX(); cellX++){
        for(int cellY = 0; cellY < parameters.getDomainY(); cellY++){
            //std::cout << "[GetRhoU] Getting RhoU at cell (" << cellX << ", " << cellY << ")" << std::endl;
            std::vector<int> latticeCoords{cellX, cellY};
            float density;
            std::vector<float> velocity(2, 0);
            getDistributions(latticeCoords, distributions);
            //std::cout << "[GetRhoU] Got distributions" << std::endl;
            getRhoUfromDistributions(distributions, density, velocity);
            //std::cout << "[GetRhoU] Got rhoU" << std::endl;

            densityF[(cellY * parameters.getDomainX() + cellX)] = density;
            velocityF[(cellY * parameters.getDomainX() + cellX) * 2] = velocity[0];
            velocityF[(cellY * parameters.getDomainX() + cellX) * 2 + 1] = velocity[1];
        }
    }
}
