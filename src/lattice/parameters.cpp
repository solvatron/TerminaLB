#include "parameters.hpp"

int Parameters::getDomainX() const{
    return domainX;
}

int Parameters::getDomainY() const{
    return domainY;
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
    return viscosity/(1./3.) + 0.5;
}
