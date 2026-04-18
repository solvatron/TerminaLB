#pragma once

#include <vector>

class Lattice;

class BoundaryBuffer{
public:
    void initializeBoundaryBuffer(int domainX, int domainY); //Done
    void setBoundaryBuffer(const int boundary, const Lattice& lattice);
    void setInletProfile(const int boundary, const std::vector<float>& velocityProfile);
    void setInlet(const int boundary, const std::vector<float>& velocity); //Done
    void setOutlet(const int boundary);
    void writeBoundaryToLattice(const Lattice& lattice, const int boundary);
private:
    std::vector<float> boundaryF;
    int domainX;
    int domainY;

    void setBoundaryBufferCell(const int boundary, const int cellN, const std::vector<float>& distributions); //Done
    void setBoundaryBufferFromLatticeCell(const Lattice& lattice, const int boundary, const int cellN); //Done
    std::vector<int> getLatticeCoords(const int boundary, const int cellN);
    std::vector<int> getBoundaryDirectionsRead(const int boundary) const; //Done
    std::vector<int> getBoundaryDirectionsWrite(const int boundary) const; //Done
};
