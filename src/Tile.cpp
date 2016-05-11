#include "Tile.h"

#include "LogicBox.hpp"

Tile::Tile() : _x(LogVal()), _y(LogVal()), _o('x') { }  //unspecified state

Tile::Tile(LogVal x, LogVal y, char operand) : _x(x), _y(y), _o(operand) { }

LogVal Tile::Calculate(LogicBox &l){    //calculate result; _x and _y have to be created by same LogicBox
    switch (_o){
        case 'a': _w = l.And(_x,_y); break;
        case 'o': _w = l.Or(_x,_y); break;
        case 'i': _w = l.Impl(_x,_y); break;
        case 'n': _w = l.Not(_x); break;
        default: _w = LogVal();
    }
    return _w;
}

LogVal Tile::GetResult(){ return _w; }

void Tile::Print(WINDOW *scr, int x, int y, LogicBox &l){
    if(_o != 'w'){  //'w' operand is wall so it will be printed in another way
        mvwaddch(scr, y*2,   x*2,   l.GetLogVal(_x));
        mvwaddch(scr, y*2,   x*2+1, l.GetLogVal(_y));
        mvwaddch(scr, y*2+1, x*2,   _o);
        mvwaddch(scr, y*2+1, x*2+1, l.GetLogVal(_w));
    }
    else{
        mvwaddch(scr, y*2,   x*2,   ' ');
        mvwaddch(scr, y*2,   x*2+1, ' ');
        mvwaddch(scr, y*2+1, x*2,   ' ');
        mvwaddch(scr, y*2+1, x*2+1, ' ');
    }
}
