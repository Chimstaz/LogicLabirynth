#include "ColorsDef.h"
#include "GlobalDefines.h"
#include CURSESPATH

//Make declaration for color pairs in curses.

void makeColorDefinitions(){
    init_pair(ColWhite, COLOR_WHITE, COLOR_BLACK);
    init_pair(ColBlack, COLOR_CYAN, COLOR_BLACK);

    init_pair(ColP1W, COLOR_WHITE, COLOR_RED);
    init_pair(ColP1B, COLOR_CYAN, COLOR_RED);
    init_pair(ColP2W, COLOR_WHITE, COLOR_GREEN);
    init_pair(ColP2B, COLOR_CYAN, COLOR_GREEN);

    init_pair(ColPreViewW, COLOR_WHITE, COLOR_BLUE);
    init_pair(ColPreViewB, COLOR_CYAN, COLOR_BLUE);

    init_pair(ColTarget, COLOR_WHITE, COLOR_GREEN);
}
