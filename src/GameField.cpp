#include "GameField.h"

#include "LogicBox.hpp"
#include "GameFieldStrings.h"

GameField::GameField(LogicBox &logic, int mapSizeX, int mapSizeY, int winSizeX, int winSizeY, int x, int y)
    :_map(logic, mapSizeX, mapSizeY, x, y)
    ,_panel(winSizeX - mapSizeX*2 - 1, winSizeY, x + mapSizeX*2 + 1, y) //messageWindow take all space that lefts after map (where map size refer to declared space for map, no actually map size after load)
    ,_input()
    ,_logic(&logic)
    ,_moveType(0)
    ,_mapStatus(MapLoadNotLoaded)
    ,panelInput(_panel.input){
    _panel.SetHeader(HEADER_STRING);
    }

MoveRaport GameField::MakeMove(){
    MoveRaport moveInfo = MoveChangeType;
    switch (_moveType) {
        case 0:
            _panel.input << MOVE_ON;
            break;
        case 1:
            _panel.input << CHANGE_VALUE_ON;
            break;
    }
    _panel.Print();

    while(moveInfo == MoveChangeType){  //while expected action is to rotate between move types
        if(_moveType == 0){ //move type 0 is changing position
            moveInfo = _makeMove();
            if(moveInfo == MoveChangeType) {
                _moveType = 1;   //if player wants to change move type
                _panel.input << CHANGE_VALUE_ON;
                _panel.Clear();
                _panel.Print();
            }
        }
        else{
            moveInfo = _modifyTile();   //move type 1 is to modify tile
            if(moveInfo == MoveChangeType){
                _moveType = 0;
                _panel.input << MOVE_ON;
                _panel.Clear();
                _panel.Print();
            }
            else if (moveInfo == MoveOK){   //if correct modified tile only option is to make calculation
                _panel.input << CALCULATE_ON;
                _panel.Clear();
                _panel.Print();
                moveInfo = _makeCalculation();
            }
        }
    }//moveInfo MoveExitKey should immediately stop move loop
    return moveInfo;
}

MoveRaport GameField::_makeMove(){
    InputInfo input;
    direction dir = DirUnknown;
    while(true){
        input = _input.ProcessInput();  //wait for key
        switch (input) {
            case InputExit: return MoveExitKey; //end immediately
            case InputAccept:
                if(_map.ChangePlayerPosition()){    //try to make move
                    if(_map.IsOnTarget()) return MoveWiner; //say if player win
                    return MoveOK;  //or only moved correctly
                }
                break;
            case InputDown: //set direction based on pressed key
                dir = DirDown;
                break;
            case InputUp:
                dir = DirUp;
                break;
            case InputLeft:
                dir = DirLeft;
                break;
            case InputRight:
                dir = DirRight;
                break;
            case InputToggleMoveType: return MoveChangeType;    //tell about need to change move type
            case InputHelp: _printHelp(1); break;
            default: break;
        }
        _map.PreMovePlayer(dir);    //show result on map
    }
}

MoveRaport GameField::_modifyTile(){
    InputInfo input;
    direction dir = DirUnknown;
    char v = 255;
    while(true){
        input = _input.ProcessInput();  //wait for key
        switch (input) {
            case InputExit: return MoveExitKey; //exit key
            case InputAccept:
                if(_map.ChangeValueUnderPlayer()) return MoveOK;    //value changed correctly
                break;
            case InputDown:
                dir = DirDown;
                break;
            case InputLeft:
                dir = DirLeft;
                break;
            case InputRight:
                dir = DirRight;
                break;
            case InputOther:    //if pressed key is different to special meaning keys probably it is logic value
                v = _input.OtherKey;    //InputHandler store last pressed key if it is ASCII one
                break;
            case InputToggleMoveType: return MoveChangeType;
            case InputHelp: _printHelp(2); break;
            default: break;
        }
        _map.PreValueChange(dir, v);    //show predicted move on map
    }
}

MoveRaport GameField::_makeCalculation(){
    InputInfo input;
    direction dirX = DirUnknown;
    direction dirY = DirUnknown;
    while(true){
        input = _input.ProcessInput();  //wait for key
        switch (input) {
            case InputExit: return MoveExitKey; //exit key
            case InputAccept:
                if(_map.Calculate()) return MoveOK; //calculation was correct
                break;
            case InputDown:     //set correct direction based on pressed keys
                dirY = DirDown;
                break;
            case InputUp:
                dirY = DirUp;
                break;
            case InputLeft:
                dirX = DirLeft;
                break;
            case InputRight:
                dirX = DirRight;
                break;
            case InputHelp: _printHelp(3); break;
            default: break;
        }
        _map.PreCalculate(dirX, dirY);  //show pre calculation view
    }
}

void GameField::GetMapWindowSize(int &x, int &y){
    _map.GetWindowSize(x, y);   //map can give information about it's window
}

void GameField::GeneratRandomMap(int mapSizeX, int mapSizeY){
    _mapStatus = _map.GenerateRandom(mapSizeX, mapSizeY);
}

void GameField::LoadMap(fstream &mapF, int mapSizeX, int mapSizeY){
    _mapStatus = _map.LoadMap(mapF, mapSizeX, mapSizeY);
}

LoadMapFileError GameField::GetMapState(){  //map status is updated on every map load
    return _mapStatus;
}

void GameField::PrintMap(){
    if(_mapStatus == MapLoadOK) _map.SimplyPrint(); //if map is correct then print it
}

void GameField::PrintMessage(){
    _panel.Print();
}

void GameField::PrintMessage(int x, int y){
    _panel.Print(x,y);
}

void GameField::ClearMessage(){
    _panel.Clear();
}

void GameField::SetHeaderMessage(string header){
    _panel.SetHeader(header);
}

void GameField::_printHelp(int phase){
    switch(_input.helpKey){
        case HelpAnd:
            _panel.input << AND_LT << _logic->PrintLogTable('a');
            break;
        case HelpOr:
            _panel.input << OR_LT << _logic->PrintLogTable('o');
            break;
        case HelpImpl:
            _panel.input << IMPL_LT << _logic->PrintLogTable('i');
            break;
        case HelpNot:
            _panel.input << NOT_LT << _logic->PrintLogTable('n');
            break;
        case HelpGeneral:
            _panel.input << GENERAL_HELP;
            break;
        case HelpNoPassableValues:
            _panel.input << NOPASSABLES <<_map.GetNoPassableVals();
            break;
        case HelpMap:
            _panel.input << MAP_HELP;
            break;
        case HelpMovePhase:
            switch (phase) {
                case 1:
                    _panel.input << MOVE_HELP;
                    break;
                case 2:
                    _panel.input << CHANGE_VALUE_HELP;
                    break;
                case 3:
                    _panel.input << CALCULATE_HELP;
                    break;
                default:
                    _panel.input << " ";
                    break;
            }
    }
    _panel.Clear();
    _panel.Print();
}
