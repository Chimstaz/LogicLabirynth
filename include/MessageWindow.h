#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include "GlobalDefines.h"
#include CURSESPATH
#include <string>
#include <sstream>

/*
ncurses window that can print messages
*/

using namespace std;

class MessageWindow{
public:
    MessageWindow(int sizeX, int sizeY, int x, int y);
    ~MessageWindow();
    void Print(int x, int y);
    void Print();
    void Clear();
    void SetHeader(string header);
    stringstream input;
    WINDOW *GetPrintingField();
private:
    int _sizeX;
    int _sizeY;
    WINDOW *_screen;
    string _header;
    int _startField;
    char *_tmpstr;
};

#endif // MESSAGEWINDOW_H
