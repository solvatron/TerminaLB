#pragma once

#include <vector>
#include <cmath>

#include "boundaryBuffer.hpp"
#include "parameters.hpp"


class Lattice {
public:
    //std::vector<float> velocityF;
    //std::vector<float> densityF;
    const int numDirections = 9;
    const float c_s = 1/sqrt(3);


    void initialize(); //Done
    void collide(); //Done
    void stream();

    int getTimeStep() const;

    void getDistributions(const std::vector<int>& latticeCoords, std::vector<float>& distributions) const; //Done
    void setDistributions(const std::vector<int>& latticeCoords, const std::vector<float>& distributions); //Done

    float getSingleDistribution(const std::vector<int>& latticeCoords, const int direction) const; //Done
    void setSingleDistribution(const std::vector<int>& latticeCoords, const int direction, const float distribution); //Done
private:
    int step = 0;

    const std::vector<std::vector<int>> latticeC = {{0, 0}, {1, 0}, { 0, 1}, {-1,  0}, {0, -1},
                                                              {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

    std::vector<float> solidF;
    std::vector<float> distributionF;
    std::vector<std::vector<int>> displacementVectors{{0, 0}, {0, 0}, {0, 0},
                                                      {0, 0}, {0, 0}, {0, 0},
                                                      {0, 0}, {0, 0}, {0, 0}};
    std::vector<float> weights{4./9., 1./9., 1./9.,  1./9.,  1./9.,  1./36., 1./36.,  1./36.,  1./36.};

    BoundaryBuffer boundaryBuffer;

    Parameters parameters;


    void initializeDistributionF(); //Done

    void getRhoUfromDistributions(const std::vector<float>& distributions, int density, std::vector<float>& velocity); //Done
    void getEqDistribution(const float density, const std::vector<float>& velocity, std::vector<float>& eqDistributions); //Done
    void relaxDistributions(const std::vector<float>& eqDistributions, std::vector<float>& distributions); //Done
    int getMemoryIndex(const std::vector<int> latticeCoords, const int direction) const;

    void incrementDisplacement();
};
