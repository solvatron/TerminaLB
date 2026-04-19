#include "renderer.hpp"
#include "../lattice/lattice.hpp"
#include <iostream>
#include <vector>

void Renderer::showRawRhoU(const Lattice& lattice) const{
    int domainX = lattice.parameters.getDomainX();
    int domainY = lattice.parameters.getDomainY();
    std::vector<float> velocity(2, 0);

    std::vector<float> densityF(domainX * domainY, 0.0f);
    std::vector<float> velocityF(domainX * domainY * 2, 0.0f);

    std::cout << "Initialized Fields" << std::endl;
    lattice.getRhoUField(densityF, velocityF);

    std::cout << "Step: " << lattice.getTimeStep() << std::endl;
    std::cout << "Density:" << std::endl;
    for(int cellY = 0; cellY < domainY; cellY++){
        for(int cellX = 0; cellX < domainX; cellX++){
            float density = densityF[cellY * domainX + cellX];
            std::cout << density << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "Velocity:" << std::endl;

    for(int cellY = 0; cellY < domainY; cellY++){
        for(int cellX = 0; cellX < domainX; cellX++){
            velocity[0]  = velocityF[(cellY * domainX + cellX) * 2];
            velocity[1]  = velocityF[(cellY * domainX + cellX) * 2 + 1];
            std::cout << "(" << velocity[0] << " " << velocity[1] << ") ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
