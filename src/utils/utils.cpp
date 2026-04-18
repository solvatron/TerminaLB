#include "utils.hpp"



int utils::reverse(const int direction){
    if(direction == 0) return 0;
    if(direction == 1) return 3;
    if(direction == 2) return 4;
    if(direction == 3) return 1;
    if(direction == 4) return 2;
    if(direction == 5) return 7;
    if(direction == 6) return 8;
    if(direction == 7) return 5;
    if(direction == 8) return 6;
    else return -1;
}
