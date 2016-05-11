#ifndef GAME_H
#define GAME_H

class LogicBox;
class GameField;
enum state : int;
enum LoadMapFileError : char;

#include <fstream>

using namespace std;

/*
Game class contains own LogicBox and GameField.
GameField need LogicBox, so first setup LogicBox.
Load/Random/Map load map and create GameField.
DoGameLoop start game loop with current loaded map.
*/

class Game
{
    public:
        Game();
        virtual ~Game();
        void DoGameLoop();
        bool SetUpLogicBox(fstream &notF, fstream &andF, fstream &orF, fstream &implF);
        state GetLogicBoxState();
        bool IsSet();
        bool LoadMap(fstream &mapF);
        bool LoadRandomMap(int mapSizeX, int mapSizeY);
        LoadMapFileError GetMapState();
        const int PanelSize = 25;
    private:
        LogicBox *_logic;
        GameField *_gameField;
        bool _creatGameField(int mapSizeX, int mapSizeY);
};

#endif // GAME_H
