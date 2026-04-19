#include<iostream>
#include "lattice/lattice.hpp"
#include "renderer/renderer.hpp"



int main(){
	Lattice lattice;
	Renderer renderer;
	std::cout << "Created lattice and renderer" << std::endl;
	lattice.initialize();
	std::cout << "Lattice initialized" << std::endl;
	renderer.showRawRhoU(lattice);
	std::cout << "Lattice rendered" << std::endl;
	while (true) {
		int numSteps;
		std::cin >> numSteps;
		std::cout << "Run " << numSteps << " steps:" << std::endl;

		for (int step = numSteps; step > 0; step--) {
			lattice.collide();
			std::cout << "Lattice collided" << std::endl;
			lattice.stream();
			std::cout << "Lattice streamed" << std::endl;
			renderer.showRawRhoU(lattice);
			std::cout << "Lattice rendered" << std::endl;
		}
	}

	return 0;
}
