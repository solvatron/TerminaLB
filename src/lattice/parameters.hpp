#pragma once

#include <vector>
class Parameters{
public:
    int getDomainX() const;
    int getDomainY() const;

    bool isInlet(const int boundary) const;
    bool isOutlet(const int boundary) const;
    float getInletVelocity(const int boundary) const;

    float getLatticeU() const;
    float getDx() const;
    float getUin() const;
    float getVisosity() const;

    float getRelaxationTime() const;
private:
    int domainX;
    int domainY;

    std::vector<int> boundaryType{2, 0, 1, 0}; //0 = BB, 1 = inflow, 2 = outflow
    std::vector<float> inletVelocity{0, 0, 1, 0};

    float latticeU = 0.1;
    float dx = 1;
    float Uin = 1;
    float viscosity = 1;
};
