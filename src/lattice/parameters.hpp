#pragma once

class Parameters{
public:
    int getDomainX() const;
    int getDomainY() const;

    float getLatticeU() const;
    float getDx() const;
    float getUin() const;
    float getVisosity() const;

    float getRelaxationTime() const;
private:
    int domainX;
    int domainY;

    float latticeU = 0.1;
    float dx = 1;
    float Uin = 1;
    float viscosity = 1;
};
