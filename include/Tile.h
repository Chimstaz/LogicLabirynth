#ifndef TILE_H
#define TILE_H

class LogicBox;

#include "GlobalDefines.h"
#include CURSESPATH
#include "LogVal.h"

/*
Tile contains two operands x and y, and indicator of logic function that will be used to calculate result
*/

class Tile {
public:
    Tile();
    Tile(LogVal x, LogVal y, char operand);
    LogVal Calculate(LogicBox &l);
    LogVal GetResult();
    void Print(WINDOW *scr, int x, int y, LogicBox &l);

    LogVal _x;
    LogVal _y;
    char _o;
private:
    LogVal _w;
};

#endif // TILE_H
