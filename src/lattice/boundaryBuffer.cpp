#include "boundaryBuffer.hpp"
#include "lattice.hpp"
#include "../utils/utils.hpp"

#include <iostream>
#include <vector>

void BoundaryBuffer::initializeBoundaryBuffer(int _domainX, int _domainY){
    domainX = _domainX;
    domainY = _domainY;
    boundaryF.resize((2 * domainX + 2 * domainY) * 3, 0);
}

void BoundaryBuffer::setBoundaryBuffer(const Lattice& lattice){
    for(int boundary = 1; boundary <= 4; boundary++){
        std::cout << "[BoundaryBuffer] Setting BoundaryBuffer for boundary " << boundary << std::endl;
        int boundaryLength;
        if(boundary%2 == 1) boundaryLength = domainY;
        else boundaryLength = domainX;
        for(int cellN = 0; cellN < boundaryLength; cellN++){
            setBoundaryBufferFromLatticeCell(boundary, lattice, cellN);
        }
    }
}

void BoundaryBuffer::writeBoundaryBuffer(Lattice& lattice) const{
    for(int boundary = 1; boundary <= 4; boundary++){
        std::cout << "[BoundaryBuffer] Writing BoundaryBuffer for boundary " << boundary << std::endl;
        int boundaryLength;
        if(boundary%2 == 1) boundaryLength = domainY;
        else boundaryLength = domainX;
        for(int cellN = 0; cellN < boundaryLength; cellN++){
            setLatticeCellFromBoundaryBuffer(boundary, lattice, cellN);
        }
    }
}

void BoundaryBuffer::setInlet(const int boundary, Lattice& lattice, const std::vector<float>& velocityProfile) const{
    
}

void BoundaryBuffer::setInlet(const int boundary, Lattice& lattice, const float velocity) const{
    std::vector<float> velocityProfile;
    std::vector<int> velocityVec = lattice.latticeC[utils::reverse(boundary)];
    int boundaryLength;
    if(boundary%2 == 1) boundaryLength = domainY;
    else boundaryLength = domainX;
        velocityProfile.resize(2. * boundaryLength, 0.0f);
        for(int i = 0; i < boundaryLength; i++){
            velocityProfile[2.*i] = (float)velocityVec[0] * velocity;
            velocityProfile[2.*i + 1] = (float)velocityVec[1] * velocity;
        }

    setInlet(boundary, lattice, velocityProfile);
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

void BoundaryBuffer::setBoundaryBufferFromLatticeCell(const int boundary, const Lattice& lattice, const int cellN){
    std::vector<float> distributions (3, 0.0f);
    std::vector<int> latticeCoords = getLatticeCoords(boundary, cellN);
    std::vector<int> directions = getBoundaryDirectionsRead(boundary);
    for(int i = 0; i < directions.size(); i++){
        distributions[i] = lattice.getSingleDistribution(latticeCoords, directions[i]);
    }
    setBoundaryBufferCell(boundary, cellN, distributions);
}

void BoundaryBuffer::getBoundaryBufferCell(const int boundary, const int cellN, std::vector<float>& distributions) const{
    distributions.resize(3, 0);
    int index = 0;
    if(boundary == 2) index = domainY;
    if(boundary == 3) index = domainY + domainX;
    if(boundary == 4) index = 2. * domainY + domainX;
    index += cellN;
    index *= 3;
    distributions[0] = boundaryF[index];
    distributions[1] = boundaryF[index+1];
    distributions[2] = boundaryF[index+2];
}

void BoundaryBuffer::setLatticeCellFromBoundaryBuffer(const int boundary, Lattice& lattice, const int cellN) const{
    std::vector<float> distributions (3, 0.0f);
    std::vector<int> latticeCoords = getLatticeCoords(boundary, cellN);
    std::vector<int> directions = getBoundaryDirectionsWrite(boundary);
    //std::cout << "[BoundaryBuffer] Setting cell " << cellN << " at boundary " << boundary << std::endl;
    //std::cout << "[BoundaryBuffer] The directions are: " << directions[0] << ", " << directions[1] << ", " << directions[2] << std::endl;
    getBoundaryBufferCell(boundary, cellN, distributions);
    //std::cout << "[BoundaryBuffer] Got BoundaryBuffer distributions" << std::endl;
    for(int i = 0; i < directions.size(); i++){
        lattice.setSingleDistribution(latticeCoords, directions[i], distributions[i]);
        //std::cout << "[BoundaryBuffer] Set direction " << directions[i] << std::endl;
    }
}

std::vector<int> BoundaryBuffer::getLatticeCoords(const int boundary, const int cellN) const{
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
