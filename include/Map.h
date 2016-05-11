#ifndef MAP_H
#define MAP_H

class LogicBox;

enum LoadMapFileError : char { MapLoadOK, MapLoadNoLogicOrScreen, MapLoadTooSmallScreen, MapLoadNoSize, MapLoadReadError, MapLoadBadLogVal, MapLoadBadOperand, MapLoadBadNoPassableVals, MapLoadNotLoaded};


#include "GlobalDefines.h"
#include "Tile.h"
#include <vector>
#include <set>
#include <fstream>
#include <string>

using namespace std;

class Map{
public:
    Map(LogicBox &logic, int sizeX, int sizeY, int x, int y);
    ~Map();
    LoadMapFileError GenerateRandom(int sizeX, int sizeY);
    LoadMapFileError LoadMap(fstream &file, int sizeX, int sizeY);
    string GetNoPassableVals();

    void SimplyPrint();

    void PreMovePlayer(direction dir);
    bool ChangePlayerPosition();

    void PreValueChange(direction part, char v);
    bool ChangeValueUnderPlayer();

    void PreCalculate(direction x, direction y);
    bool Calculate();

    void ReverseColors();
    void GetWindowSize(int &x, int &y);
    void GetMapSize(int &x, int &y);
    bool IsOnTarget();
private:
    LogicBox *_logic;
    int _sizeX;
    int _sizeY;
    int _colorMod;
    WINDOW *_screen;
    vector<vector <Tile> > _tiles;
    set<unsigned int> _noPassableVals;
    pair<int,int> _player1Pos;
    pair<int,int> _targetPos;
    void _getXYfromPlayer(int &x, int &y);
    bool _isColorWhite(int x, int y);
    void _clearPreValue(int phase);
    void _processPlayer();

    int _playerToProcess;
    direction _preMoveDir;
    direction _preValueDir;
    direction _preCalculateXDir;
    direction _preCalculateYDir;
    char _preValue;
};

#endif // MAP_H
