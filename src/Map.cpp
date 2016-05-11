#include "Map.h"

#include <chrono>
#include "ColorsDef.h"
#include "Sleep.h"
#include CURSESPATH
#include <random>
#include "LogicBox.hpp"
#include <queue>
#include <set>
#include <tuple>
#include <fstream>

Map::Map(LogicBox &logic, int sizeX, int sizeY, int x, int y)
    :_logic(&logic)
    ,_colorMod(1)
	,_screen(newwin(sizeY*2, sizeX*2, y, x))
	,_noPassableVals()
	,_playerToProcess(1)
	,_preMoveDir(DirUnknown)
	,_preValueDir(DirUnknown)
	,_preCalculateXDir(DirUnknown)
	,_preCalculateYDir(DirUnknown)
	,_preValue(0) { }

Map::~Map(){
    delwin(_screen);
}

void Map::ReverseColors(){
    _colorMod = (_colorMod + 1)%2;
}

bool Map::_isColorWhite(int x, int y){
    return (x+y+_colorMod)%2;
}

void Map::_getXYfromPlayer(int &x, int &y){
    x = _player1Pos.first;
    y = _player1Pos.second;
}

void Map::GetWindowSize(int &x, int &y){
    x = _screen->_maxx;
    y = _screen->_maxy;
}

void Map::GetMapSize(int &x, int &y){
    x = _sizeX;
    y = _sizeY;
}

LoadMapFileError Map::GenerateRandom(int sizeX, int sizeY){
    if(_logic == NULL || _screen == NULL) return MapLoadNoLogicOrScreen;
    _sizeX = sizeX;
    _sizeY = sizeY;
    for(int i = _tiles.size()-1; i >= 0; --i) _tiles[i].clear();
    _tiles.clear();
    vector<Tile> col;
    for(int i=0; i < _sizeY; ++i) col.push_back(Tile());
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 randGen(seed);
    for(int i=0; i < _sizeX; ++i){
        char op;
        _tiles.push_back(col);
        for(int j=0; j < _sizeY; ++j){
            switch ((unsigned int)(((double)randGen()/(double)randGen.max())*5)){
                case 0: op = 'n'; break;
                case 1: op = 'o'; break;
                case 2: op = 'a'; break;
                case 3: op = 'i'; break;
                case 4: op = 'w'; break;
            }
            _tiles[i][j] = Tile(_logic->CreatRandomLogVal(), _logic->CreatRandomLogVal(), op);
            _tiles[i][j].Calculate(*_logic);
        }
    }
    _noPassableVals.clear();
    _noPassableVals.insert(_logic->CreatRandomLogVal().GetVal());
    _player1Pos = pair<int,int>(0,0);
    _targetPos = pair<int,int>(_sizeX -1, _sizeY-1);
    return MapLoadOK;
}

LoadMapFileError Map::LoadMap(fstream &file, int sizeX, int sizeY){
    if(_logic == NULL || _screen == NULL) return MapLoadNoLogicOrScreen;
    _sizeX = sizeX;
    _sizeY = sizeY;
    for(int i = _tiles.size()-1; i >= 0; --i) _tiles[i].clear();
    _tiles.clear();
    vector<Tile> col;
    for(int i=0; i < _sizeY; ++i) col.push_back(Tile());
    for(int i=0; i < _sizeX; ++i) _tiles.push_back(col);

    char x, y;
    void SetNoPassableVals(vector<LogVal> &noPassable);

    for(int i=0; i < _sizeY; ++i){
        for(int j=0; j < _sizeX; ++j){
            if(!(file >> x >> y)) return MapLoadReadError;
            _tiles[j][i] = Tile(_logic->CreatLogVal(x), _logic->CreatLogVal(y), 'w');
            if(!_tiles[j][i]._x.IsSet() || !_tiles[j][i]._y.IsSet()) return MapLoadBadLogVal;
        }
        for(int j=0; j < _sizeX; ++j){
            if(!(file >> x >> y)) return MapLoadReadError;
            switch (x){
                case 'n': break;
                case 'o': break;
                case 'a': break;
                case 'i': break;
                case 'w': break;
                default: return MapLoadBadOperand;
            }
            switch (y){
                case 't': _targetPos.first = j; _targetPos.second = i; break;
                case 'p': _player1Pos.first = j; _player1Pos.second = i; break;
            }
            _tiles[j][i]._o = x;
            _tiles[j][i].Calculate(*_logic);
        }
    }
    _noPassableVals.clear();
    string noPassable;
    LogVal t;
    if(!(file >> noPassable)) return MapLoadReadError;
    for(auto it = noPassable.begin(); it != noPassable.end(); ++it){
        t = _logic->CreatLogVal(*it);
        if(!t.IsSet()) return MapLoadBadNoPassableVals;
        _noPassableVals.insert(t.GetVal());
    }

    return MapLoadOK;
}

string Map::GetNoPassableVals(){
    string out;
    for(auto it = _noPassableVals.begin(); it != _noPassableVals.end(); ++it){
        out.push_back(_logic->GetLogVal(*it));
    }
    return out;
}

void Map::SimplyPrint(){
	wattrset(_screen, A_NORMAL);
    int i;
    int j;
    for(i = 0; i < _sizeX; ++i){
        for(j = 0; j < _sizeY; ++j){
            if(_isColorWhite(i,j)) wattron(_screen, COLOR_PAIR(ColWhite));
            else wattron(_screen, COLOR_PAIR(ColBlack));
            _tiles[i][j].Print(_screen, i, j, *_logic);
        }
    }

    i = _targetPos.first;
    j = _targetPos.second;
    wattron(_screen, COLOR_PAIR(ColTarget));
    _tiles[i][j].Print(_screen, i, j, *_logic);

    i = _player1Pos.first;
    j = _player1Pos.second;
    if(_isColorWhite(i,j)) wattron(_screen, COLOR_PAIR(ColP1W));
    else wattron(_screen, COLOR_PAIR(ColP1B));
    wattron(_screen, A_BOLD);
    _tiles[i][j].Print(_screen, i, j, *_logic);

    refresh();
    wrefresh(_screen);
}

void Map::PreMovePlayer(direction dir){
    wattrset(_screen, A_NORMAL);
    _clearPreValue(1);
    _preMoveDir = dir;
    int x,y;
    _getXYfromPlayer(x, y);
    switch (dir){
        case DirUp:
            --y;
            if(y<0){
                _preMoveDir = DirUnknown;
                return;
            }
            break;
        case DirLeft:
            --x;
            if(x<0){
                _preMoveDir = DirUnknown;
                return;
            }
            break;
        case DirDown:
            ++y;
            if(y>=_sizeY){
                _preMoveDir = DirUnknown;
                return;
            }
            break;
        case DirRight:
            ++x;
            if(x>=_sizeX){
                _preMoveDir = DirUnknown;
                return;
            }
            break;
        default:
            _preMoveDir = DirUnknown;
            return;
    }

    if(_isColorWhite(x,y))wattron(_screen, COLOR_PAIR(ColPreViewW));
    else wattron(_screen, COLOR_PAIR(ColPreViewB));

    if((_noPassableVals.find(_tiles[x][y].GetResult().GetVal()) != _noPassableVals.end()) || _tiles[x][y]._o == 'w'){
        _preMoveDir = DirUnknown;
        return;
    }

    wattroff(_screen, A_BOLD);
    _tiles[x][y].Print(_screen, x, y, *_logic);
    refresh();
    wrefresh(_screen);
}

bool Map::ChangePlayerPosition(){
    wattrset(_screen, A_NORMAL);
    int x,y;
    _getXYfromPlayer(x, y);
    if(_preMoveDir == DirUnknown) return false;

    if(_isColorWhite(x,y))wattron(_screen, COLOR_PAIR(ColWhite));
    else wattron(_screen, COLOR_PAIR(ColBlack));
    if(x == _targetPos.first && y == _targetPos.second) wattron(_screen, COLOR_PAIR(ColTarget));
    wattroff(_screen,A_BOLD);
    _tiles[x][y].Print(_screen, x, y, *_logic);

    switch (_preMoveDir){
        case DirUp:
            --y;
            break;
        case DirLeft:
            --x;
            break;
        case DirDown:
            ++y;
            break;
        case DirRight:
            ++x;
            break;
        default: return false;
    }
    _processPlayer();

    _player1Pos.first = x;
    _player1Pos.second = y;
    if(_isColorWhite(x,y))wattron(_screen, COLOR_PAIR(ColP1W));
    else wattron(_screen, COLOR_PAIR(ColP1B));

    wattron(_screen, A_BOLD);
    _tiles[x][y].Print(_screen, x, y, *_logic);
    refresh();
    wrefresh(_screen);
    return true;
}

void Map::PreValueChange(direction part, char v){
    _clearPreValue(2);
    wattrset(_screen, A_NORMAL);
    if(part != DirUnknown) _preValueDir = part;
    int x,y;
    _getXYfromPlayer(x, y);

    if(_isColorWhite(x,y))wattron(_screen, COLOR_PAIR(ColP1W));
    else wattron(_screen, COLOR_PAIR(ColP1B));

    LogVal t;

    switch (_preValueDir){
        case DirDown:
            if(v == (char)255) v = _tiles[x][y]._o;
            _preValue = v;
            if(v != 'a' && v != 'o' && v != 'i' && v != 'n'){
                wattron(_screen, A_ITALIC|A_BOLD);
                _preValue = 0;
            }
            x = x*2;
            y = y*2+1;
            break;
        case DirLeft:
            if(v == (char)255) v = _logic->GetLogVal(_tiles[x][y]._x);
            _preValue = v;
            if(!_logic->SetLogVal(t,v)){
                wattron(_screen, A_ITALIC|A_BOLD);
                _preValue = 0;
            }
            x = x*2;
            y = y*2;
            break;
        case DirRight:
            if(v == (char)255) v = _logic->GetLogVal(_tiles[x][y]._y);
            _preValue = v;
            if(!_logic->SetLogVal(t,v)){
                wattron(_screen, A_ITALIC|A_BOLD);
                _preValue = 0;
            }
            x = x*2+1;
            y = y*2;
            break;
        default:
            _preValue = 0;
            return;
    }
    mvwaddch(_screen, y, x, v);
    refresh();
    wrefresh(_screen);
}

bool Map::ChangeValueUnderPlayer(){
    if(_preValueDir == DirUnknown || _preValue == 0) return false;
    int x,y;
    _getXYfromPlayer(x, y);

    LogVal t;

    switch (_preValueDir){
        case DirDown:
            _tiles[x][y]._o = _preValue;
            break;
        case DirLeft:
            if(!_logic->SetLogVal(t,_preValue)) return false;
            _tiles[x][y]._x = t;
            break;
        case DirRight:
            if(!_logic->SetLogVal(t,_preValue)) return false;
            _tiles[x][y]._y = t;
            break;
        default: return false;
    }
    _processPlayer();
    _clearPreValue(2);
    return true;
}

void Map::PreCalculate(direction x, direction y){
    wattrset(_screen, A_NORMAL);
    _clearPreValue(3);
    _preCalculateXDir = x;
    _preCalculateYDir = y;
    int px,py;
    _getXYfromPlayer(px, py);
    if(_isColorWhite(px,py))wattron(_screen, COLOR_PAIR(ColP1W));
    else wattron(_screen, COLOR_PAIR(ColP1B));
    switch (x){
        case DirLeft:
            px = px*2;
            break;
        case DirRight:
            px = px*2+1;
            break;
        default:
            _preCalculateXDir = DirUnknown;
            _preCalculateYDir = DirUnknown;
            return;
    }
    switch (y){
        case DirUp:
            py=py*2;
            break;
        case DirDown:
            py=py*2+1;
            break;
        default:
            _preCalculateXDir = DirUnknown;
            _preCalculateYDir = DirUnknown;
            return;
    }
    mvwaddch(_screen, py, px, ' ');
    refresh();
    wrefresh(_screen);
}

bool Map::Calculate(){
    ReverseColors();
    wattrset(_screen, A_NORMAL);
    int pX, pY;
    _getXYfromPlayer(pX, pY);

    int dirY, dirX;

    switch (_preCalculateYDir){
        case DirUp: dirY = -1; break;
        case DirDown: dirY = 1; break;
        default: return false;
    }
    switch (_preCalculateXDir){
        case DirLeft: dirX = -1; break;
        case DirRight: dirX = 1; break;
        default: return false;
    }

    int drawOrder = 0;

    queue<tuple<int,int,int> > tilesQueue;
    set<tuple<int, int> > processedTiles;
    LogVal result;

    tilesQueue.push(make_tuple (pX, pY, drawOrder));
    processedTiles.insert(make_tuple (pX, pY));

    while(!tilesQueue.empty()){
        if(get<2>(tilesQueue.front()) > drawOrder){
            refresh();
            wrefresh(_screen);
            MySleep(max(500- 20*drawOrder, 20));
            ++drawOrder;
        }
        pX = get<0>(tilesQueue.front());
        pY = get<1>(tilesQueue.front());
        result = _tiles[pX][pY].Calculate(*_logic);
        if(_isColorWhite(pX,pY)) wattron(_screen, COLOR_PAIR(ColWhite));
        else wattron(_screen, COLOR_PAIR(ColBlack));
        if(pX == _targetPos.first && pY == _targetPos.second){
            wattron(_screen, COLOR_PAIR(ColTarget));
            _tiles[pX][pY].Print(_screen, pX, pY, *_logic);
        }
        else if(pX == _player1Pos.first && pY == _player1Pos.second){
            if(_isColorWhite(pX,pY))wattron(_screen, COLOR_PAIR(ColP1W));
            else wattron(_screen, COLOR_PAIR(ColP1B));
            wattron(_screen, A_BOLD);
            _tiles[pX][pY].Print(_screen, pX, pY, *_logic);
            wattroff(_screen, A_BOLD);
        }
        else _tiles[pX][pY].Print(_screen, pX, pY, *_logic);


        pX += dirX;
        if(pX < _sizeX && pX >= 0 && pY < _sizeY && pY >= 0 && _tiles[pX][pY]._o != 'w'){
            _tiles[pX][pY]._x = result;
            if(processedTiles.insert(make_tuple (pX, pY)).second)   //if tile with position pX, pY was not yet inserted then second in return value is true
                tilesQueue.push(make_tuple (pX, pY, get<2>(tilesQueue.front()) + 1));
        }
        pX -= dirX;
        pY += dirY;
        if(pX < _sizeX && pX >= 0 && pY < _sizeY && pY >= 0 && _tiles[pX][pY]._o != 'w'){
            _tiles[pX][pY]._y = result;
            if(processedTiles.insert(make_tuple (pX, pY)).second)
                tilesQueue.push(make_tuple (pX, pY, get<2>(tilesQueue.front()) + 1));
        }
        tilesQueue.pop();
    }

    refresh();
    wrefresh(_screen);
    SimplyPrint();
    _processPlayer();
    return true;
}

bool Map::IsOnTarget(){
    return (_targetPos.first == _player1Pos.first && _targetPos.second == _player1Pos.second);
}

void Map::_clearPreValue(int phase){
    SimplyPrint();
}


void Map::_processPlayer(){
    _playerToProcess = 1;
    _preCalculateXDir = DirUnknown;
    _preCalculateYDir = DirUnknown;
    _preMoveDir = DirUnknown;
    _preValueDir = DirUnknown;
    _preValue = 0;
}
