#include "parameters.hpp"
#include <iostream>


int Parameters::getDomainX() const{
    return domainX;
}

int Parameters::getDomainY() const{
    return domainY;
}

bool Parameters::isInlet(const int boundary) const{
    return boundaryType[boundary] == 1;
}

bool Parameters::isOutlet(const int boundary) const{
    return boundaryType[boundary] == 2;
}

float Parameters::getInletVelocity(const int boundary) const{
    if(!isInlet(boundary)) std::cout<< "Warning: Requested inlet velocity for non-inlet type boundary" << std::endl;
    return inletVelocity[boundary];
}

float Parameters::getLatticeU() const{
    return latticeU;
}

float Parameters::getDx() const{
    return dx;
}

float Parameters::getUin() const{
    return Uin;
}

float Parameters::getVisosity() const{
    return viscosity;
}

float Parameters::getRelaxationTime() const{
    float viscosityConversion = latticeU * dx;
    return viscosity / viscosityConversion / (1./3.) + 0.5;
}
