#include "MessageWindow.h"

MessageWindow::MessageWindow(int sizeX, int sizeY, int x, int y)
    :_sizeX(sizeX)
    ,_sizeY(sizeY)
    ,_screen (newwin(sizeY, sizeX, y, x))
    ,_startField(0) {
    _tmpstr = new char[sizeY*sizeX];
    }

MessageWindow::~MessageWindow(){
    delwin(_screen);
    delete[] _tmpstr;
}

void MessageWindow::Print(int x, int y){
    wmove(_screen, y,x);
    Print();
}

void MessageWindow::Print(){
    int x,y;
    getyx(_screen, y, x);
    int charleft = _sizeX - x + _sizeX*(_sizeY - y - 2);
    while(input.get(*_tmpstr)){
        input.unget();
        input.getline(_tmpstr, charleft);
        input.clear();
        wprintw (_screen, "%s\n", _tmpstr );
        getyx(_screen, y, x);
        if(y > _sizeY-2){
            wprintw (_screen, "Press any key to continue");
            refresh();
            wrefresh(_screen);
            getch();
            Clear();
            wmove(_screen, _startField,0);
            charleft = _sizeX*(_sizeY - _startField - 1);
        }
        else{
            charleft = _sizeX - x + _sizeX*(_sizeY - y - 2);
        }
        refresh();
        wrefresh(_screen);
    }
    input.clear();
}

WINDOW *MessageWindow::GetPrintingField(){
    return _screen;
}

void MessageWindow::Clear(){
    wclear(_screen);
    mvwprintw (_screen, 0,0, "%s\n", _header.c_str() );
    refresh();
    wrefresh(_screen);
    int x;
    getyx(_screen, _startField, x);
}

void MessageWindow::SetHeader(string header){
    _header = header;
}
