#ifndef GAMEFIELD_H
#define GAMEFIELD_H

class LogicBox;

#include "GlobalDefines.h"
#include "Map.h"
#include "MessageWindow.h"
#include "InputHandler.h"
#include <string>

enum MoveRaport { MoveOK, MoveExitKey, MoveError, MoveUndefinedPlayer, MoveChangeType, MoveWiner };

/*
Connect Map, MessageWindow and InputHandler.
Also pointer to LogicBox that is used by map.
Constructor create empty Map and MessageWindow.
MakeMove handle input and make specific move on Map.
Load/Generate/Map load Map.
PrintMessage on MessageWindow.
*/

class GameField{
public:
    GameField(LogicBox &logic, int mapSizeX, int mapSizeY, int winSizeX, int winSizeY, int x, int y);
    MoveRaport MakeMove();
    void GetMapWindowSize(int &x, int &y);
    LoadMapFileError GetMapState();
    void GeneratRandomMap(int mapSizeX, int mapSizeY);
    void LoadMap(fstream &mapF, int mapSizeX, int mapSizeY);
    void PrintMap();
    void PrintMessage();
    void PrintMessage(int x, int y);
    void ClearMessage();
    void SetHeaderMessage(string header);
    stringstream &panelInput;
private:
    Map _map;
    MessageWindow _panel;
    InputHandler _input;
    LogicBox *_logic;
    int _moveType;
    MoveRaport _makeMove();
    MoveRaport _modifyTile();
    MoveRaport _makeCalculation();
    LoadMapFileError _mapStatus;
    void _printHelp(int phase);
};

#endif // GAMEFIELD_H
