#include<iostream>
#include "lattice/lattice.hpp"
#include "utils/utils.hpp"



int main(){
	Lattice lattice;
	for(int direction = 0; direction < lattice.numDirections; direction++){
		int reverse = utils::reverse(direction);
		std::cout<<"Direction: " << direction << " | Reverse: " << reverse << std::endl;
	}

	return 0;
}
