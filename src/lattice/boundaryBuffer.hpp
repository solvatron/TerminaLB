#pragma once

#include <vector>

class Lattice;

class BoundaryBuffer{
public:
    void initializeBoundaryBuffer(int domainX, int domainY); //Done
    void setBoundaryBuffer(const Lattice& lattice); //Done
    void writeBoundaryBuffer(Lattice& lattice) const; //Done
    void setInlet(const int boundary, Lattice& latice, const std::vector<float>& velocityProfile) const;
    void setInlet(const int boundary, Lattice& lattice, const float velocity) const; //Done
    void setOutlet(const int boundary, Lattice& lattice);
private:
    std::vector<float> boundaryF;
    int domainX;
    int domainY;

    void setBoundaryBufferCell(const int boundary, const int cellN, const std::vector<float>& distributions); //Done
    void setBoundaryBufferFromLatticeCell(const int boundary, const Lattice& lattice, const int cellN); //Done

    void getBoundaryBufferCell(const int boundary, const int cellN, std::vector<float>& distributions) const; //Done
    void setLatticeCellFromBoundaryBuffer(const int boundary, Lattice& lattice, const int cellN) const; //Done

    std::vector<int> getLatticeCoords(const int boundary, const int cellN) const; //Done
    std::vector<int> getBoundaryDirectionsRead(const int boundary) const; //Done
    std::vector<int> getBoundaryDirectionsWrite(const int boundary) const; //Done
};
