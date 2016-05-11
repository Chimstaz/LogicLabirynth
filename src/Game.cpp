#include "Game.h"
#include "LogicBox.hpp"
#include "GameField.h"
#include "GlobalDefines.h"
#include CURSESPATH
#include <sstream>


Game::Game()
:_logic(NULL)
,_gameField(NULL)
{ }

Game::~Game()
{
    if(_logic != NULL) delete _logic;
    if(_gameField != NULL) delete _gameField;
}

bool Game::SetUpLogicBox(fstream& notF, fstream& andF, fstream& orF, fstream& implF){
    if(_logic != NULL) delete _logic;   //delete previous LogicBox
    _logic = new LogicBox(notF, andF, orF, implF);
    return _logic->IsOkay() == StateOK; //if something go wrong return false
}

state Game::GetLogicBoxState(){
    if(_logic != NULL) return _logic->IsOkay(); //if LogicBox is setup, return it state
    else return StateNotInitialized;    //return state to inform that LogicBox was not created
}

bool Game::LoadRandomMap(int mapSizeX, int mapSizeY){
    if(!_creatGameField(mapSizeX, mapSizeY)) return false;  //if creating GameField filed, there is not possible to load map
    _gameField->GeneratRandomMap(mapSizeX, mapSizeY);
    return _gameField->GetMapState() == MapLoadOK;  //if something go wrong return false
}

bool Game::LoadMap(fstream& mapF){  //pretty same like LoadRandomMap, but  mapSize is read from file
    int mapSizeX, mapSizeY;
    if(!(mapF >> mapSizeX >> mapSizeY)) return false;
    if(!_creatGameField(mapSizeX, mapSizeY)) return false;
    _gameField->LoadMap(mapF, mapSizeX, mapSizeY);
    return _gameField->GetMapState() == MapLoadOK;
}

LoadMapFileError Game::GetMapState(){
    if(_gameField != NULL) return _gameField->GetMapState();    //GameField contains MapState (after loading)
    else return MapLoadNotLoaded;
}

bool Game::_creatGameField(int mapSizeX, int mapSizeY){
    if(_logic == NULL || _logic->IsOkay() != StateOK) return false;    //can't create GameField without LogicBox
    if(_gameField != NULL){ //if GameField already exist, need to check if window is big enough to contain map
        int curWinSizeX, curWinSizeY;
        _gameField->GetMapWindowSize(curWinSizeX, curWinSizeY);
        if(curWinSizeX < mapSizeX*2 || curWinSizeY < mapSizeY*2){
            delete _gameField;  //if window is too small delete GameField
            _gameField = NULL;
        }
    }
    if(_gameField == NULL){ //if GameField does not exist create it
        if(LINES < mapSizeY*2 || COLS < mapSizeX*2 + PanelSize){
            resize_term(mapSizeY*2, mapSizeX*2 + PanelSize);   //if terminal window is too small try to resize it (+30 is for message panel
            if(LINES < mapSizeY*2 || COLS < mapSizeX*2 + PanelSize) return false;  //again check terminal size
        }
        _gameField = new GameField(*_logic, mapSizeX, mapSizeY, COLS, LINES, 0, 0); //create GameField on all screen
    }
    return true;
}

bool Game::IsSet(){
    return  GetMapState() == MapLoadOK && GetLogicBoxState() == StateOK;    //if map is ok and LogicBox is ok, than everything is setup
}

void Game::DoGameLoop(){
    if(!IsSet()) return;    //if Game is not setup, than go back
    MoveRaport moveRaport;
    int moveCount = 0;
    curs_set(0);    //disable blinking cursor
    ostringstream winMessage;
    _gameField->PrintMap(); //initial print of map
    do{
        _gameField->panelInput << moveCount << " moves used by far.";
        _gameField->ClearMessage();
        _gameField->PrintMessage();
        moveRaport = _gameField->MakeMove();    //make move on GameField
        ++moveCount;
    } while(moveRaport == MoveOK);
    if(moveRaport == MoveWiner){    //if exited because player win then print message
        winMessage.str("");
        winMessage.clear();
        winMessage << "You reach target in "<< moveCount << " moves.";
        _gameField->panelInput << winMessage.str();
        _gameField->PrintMessage();
        getch();
    }
}
