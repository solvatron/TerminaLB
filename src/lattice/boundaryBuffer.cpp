#include "boundaryBuffer.hpp"
#include "lattice.hpp"
#include "../utils/utils.cpp"

#include <vector>

void BoundaryBuffer::initializeBoundaryBuffer(int _domainX, int _domainY){
    domainX = _domainX;
    domainY = _domainY;
    boundaryF.resize((2. * domainX + 2. * domainY) * 3.);
}

void BoundaryBuffer::setBoundaryBuffer(const int boundary, const Lattice& lattice){
    if(boundary == 1){
        for(int cellN = 0; cellN < domainY; cellN++){
            std::vector<int> latticeCoords = {domainX-1, cellN};
            std::vector<float> distributions(lattice.numDirections, 0);
            lattice.getDistributions(latticeCoords, distributions);

        }
    }
}

void BoundaryBuffer::setInletProfile(const int boundary, const std::vector<float>& velocityProfile){
    
}

void BoundaryBuffer::setInlet(const int boundary, const std::vector<float>& velocity){
    std::vector<float> velocityProfile;
    if(boundary%2 == 1){
        velocityProfile.resize(2. * domainY, 0.0f);
        for(int i = 0; i < domainY; i++){
            velocityProfile[2.*i] = velocity[0];
            velocityProfile[2.*i + 1] = velocity[1];
        }
    }

    if(boundary%2 == 0){
        velocityProfile.resize(2. * domainX, 0.0f);
        for(int i = 0; i < domainX; i++){
            velocityProfile[2.*i] = velocity[0];
            velocityProfile[2.*i + 1] = velocity[1];
        }
    }
    
    setInletProfile(boundary, velocityProfile);
}

void BoundaryBuffer::setBoundaryBufferCell(const int boundary, const int cellN, const std::vector<float>& distributions){
    int index = 0;
    if(boundary == 2) index = domainY;
    if(boundary == 3) index = domainY + domainX;
    if(boundary == 4) index = 2. * domainY + domainX;
    index += cellN;
    index *= 3;
    boundaryF[index]   = distributions[0];
    boundaryF[index+1] = distributions[1];
    boundaryF[index+2] = distributions[2];
}

void BoundaryBuffer::setBoundaryBufferFromLatticeCell(const Lattice& lattice, const int boundary, const int cellN){
    std::vector<float> distributions (3, 0.0f);
    std::vector<int> latticeCoords = getLatticeCoords(boundary, cellN);
    std::vector<int> directions = getBoundaryDirectionsRead(boundary);
    for(int i = 0; i < directions.size(); i++){
        distributions[i] = lattice.getSingleDistribution(latticeCoords, directions[i]);
    }
    setBoundaryBufferCell(boundary, cellN, distributions);
}

std::vector<int> BoundaryBuffer::getLatticeCoords(const int boundary, const int cellN){
    if(boundary == 1) return std::vector<int> {domainX-1, cellN};
    if(boundary == 2) return std::vector<int> {cellN, 0};
    if(boundary == 3) return std::vector<int> {0, cellN};
    if(boundary == 4) return std::vector<int> {cellN, domainY-1};
    else return std::vector<int> (2, -1);
}

std::vector<int> BoundaryBuffer::getBoundaryDirectionsRead(const int boundary) const{
    if(boundary == 1) return std::vector<int>{8, 1, 5};
    if(boundary == 2) return std::vector<int>{5, 2, 6};
    if(boundary == 3) return std::vector<int>{6, 3, 7};
    if(boundary == 4) return std::vector<int>{7, 4, 8};
    else return std::vector<int>{0, 0, 0};
}

std::vector<int> BoundaryBuffer::getBoundaryDirectionsWrite(const int boundary) const{
    std::vector<int> writeDirections (3, 0);
    std::vector<int> readDirections = getBoundaryDirectionsRead(boundary);
    for(int i = 0; i < 3; i++){
        writeDirections[i] = utils::reverse(readDirections[i]);
    }
    return writeDirections;
}
