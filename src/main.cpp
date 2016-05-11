#include "GlobalDefines.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include CURSESPATH

#include "LogVal.h"
#include "ColorsDef.h"
#include "LogicBox.hpp"
#include "Tile.h"
#include "Map.h"
#include "MessageWindow.h"
#include "GameField.h"
#include "Game.h"

using namespace std;

void help();
void errorLoadingLogicBox(state stateOfLogicBox);
void errorLoadingMap(LoadMapFileError stateOfMap);

int main(int argc, char **argv){
    string notFile = "not.txt";
    string andFile = "and.txt";
    string orFile = "or.txt";
    string implFile = "impl.txt";
    string mapFile = "";
    bool random = true;
    int sizeX = 20;
    int sizeY = 10;

    string tmpstr = "";

    for (int i = 1; i < argc; ++i){
        tmpstr = argv[i];
        if(tmpstr == "-h") {
            help();
            return 0;
        }
        else if(tmpstr == "--not"){
            ++i;
            if(i >= argc) {
                cout << "No file path after --not.\n";
                return 1;
            }
            notFile = argv[i];
        }
        else if(tmpstr == "--and"){
            ++i;
            if(i >= argc) {
                cout << "No file path after --and.\n";
                return 1;
            }
            andFile = argv[i];
        }
        else if(tmpstr == "--or"){
            ++i;
            if(i >= argc) {
                cout << "No file path after --or.\n";
                return 1;
            }
            orFile = argv[i];
        }
        else if(tmpstr == "--impl"){
            ++i;
            if(i >= argc) {
                cout << "No file path after --impl.\n";
                return 1;
            }
            implFile = argv[i];
        }
        else if(tmpstr == "--map"){
            ++i;
            random = false;
            if(i >= argc) {
                cout << "No file path after --map.\n";
                return 1;
            }
            mapFile = argv[i];
        }
        else if(tmpstr == "-x"){
            ++i;
            if(i >= argc) {
                cout << "No map size after -x.\n";
                return 1;
            }
            sizeX = atoi(argv[i]);
        }
        else if(tmpstr == "-y"){
            ++i;
            if(i >= argc) {
                cout << "No map size after -y.\n";
                return 1;
            }
            sizeY = atoi(argv[i]);
        }
    }

    fstream m;
    streampos startMap;
    if(random == false){
        m.open(mapFile);
        if(!m.is_open()){
            cout << "Could not open " << mapFile << ".\n";
            return 1;
        }
        do{
            m >> tmpstr; //"STARTMAP" -- it stands that here start map
            if(tmpstr == "LOGICFILES"){
                if(!(m >> notFile >> orFile >> andFile >> implFile)){
                    cout << "Could not load name of logic files from " << mapFile << ".\n";
                    return 1;
                }
            }
        }while(tmpstr != "STARTMAP");
        startMap = m.tellg();
    }
    fstream n,o,a,i;
    n.open(notFile);
    o.open(orFile);
    a.open(andFile);
    i.open(implFile);
    int errorReturn = 0;
    if(!n.is_open()) {cout << "Could not open " << notFile << ".\n"; errorReturn = 1;}
    if(!o.is_open()) {cout << "Could not open " << orFile << ".\n";  errorReturn = 1;}
    if(!a.is_open()) {cout << "Could not open " << notFile << ".\n"; errorReturn = 1;}
    if(!i.is_open()) {cout << "Could not open " << notFile << ".\n"; errorReturn = 1;}
    if(errorReturn!= 0) {
        n.close();
        o.close();
        a.close();
        i.close();
        return errorReturn;
    }

	initscr();			/* Start curses mode 		  */
	start_color();			/* Start the color functionality */
	cbreak();
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
	noecho();

    makeColorDefinitions();

    Game MyGame;

    if(!(MyGame.SetUpLogicBox(n,a,o,i))) errorReturn = 2;

    n.close();
    o.close();
    a.close();
    i.close();

    if(errorReturn != 0){
        errorLoadingLogicBox(MyGame.GetLogicBoxState());
        return errorReturn;
    }
    if(random == true){
        if(!(MyGame.LoadRandomMap(sizeX, sizeY))) errorReturn = 3;
    }
    else{
        if(!(MyGame.LoadMap(m))) errorReturn = 3;;
    }

    if(errorReturn != 0){
        errorLoadingMap(MyGame.GetMapState());
        return errorReturn;
    }

    MyGame.DoGameLoop();

	endwin();			/* End curses mode		  */

    return 0;
}

void help(){
    cout << "This was printed by -h argument.\n";
    cout << "To set logic tables use:\n";
    cout << "--not, --and, --or, --impl\n";
    cout << "Followed by name of file\n";
    cout << "-x number and -y number set map size.\n";
    cout << "--map followed by name load specific map file.\n";
}


void errorLoadingLogicBox(state stateOfLogicBox){
    endwin();			/* End curses mode		  */
    string name;
    if(stateOfLogicBox >= StateImplFiledRead) {
        name = "implication";
        stateOfLogicBox = (state)(stateOfLogicBox - StateImplFiledRead + StateFiledRead);
    }
    else if(stateOfLogicBox >= StateOrFiledRead) {
        name = "or";
        stateOfLogicBox = (state)(stateOfLogicBox - StateOrFiledRead + StateFiledRead);
    }
    else if(stateOfLogicBox >= StateAndFiledRead) {
        name = "and";
        stateOfLogicBox = (state)(stateOfLogicBox - StateAndFiledRead + StateFiledRead);
    }
    else {
        name = "not";
        stateOfLogicBox = (state)(stateOfLogicBox - StateNotFiledRead + StateFiledRead);
    }
    switch (stateOfLogicBox) {
        case StateFiledRead:
            cout << "Filed to read from "<< name <<" file.\n";
            break;
        case StateTooFewArg:
            cout << "Too few logic values in one of the "<< name <<" file lines.\n";
            break;
        case StateTooManyArg:
            cout << "Too many logic values in one of the "<< name <<" file lines.\n";
            break;
        case StateTooManyDef:
            cout << "More then one definition for one of logic values in "<< name <<" file.\n";
            break;
        case StateTooFewDef:
            cout << "Not every logic value has it "<< name <<" definition.\n";
            break;
        case StateNotDefVal:
            cout << "Using logic value in "<< name <<" that was not defined in \"not\" file.\n";
            break;
        default:
            break;
    }

}

void errorLoadingMap(LoadMapFileError stateOfMap){
    endwin();			/* End curses mode		  */
    switch (stateOfMap) {
        case MapLoadNoLogicOrScreen:
        case MapLoadTooSmallScreen:
        case MapLoadNoSize:
        case MapLoadNotLoaded:
            cout << "There was error in setup screen.\n";
            break;
        case MapLoadReadError:
            cout << "Filed to read from map file.\n";
            break;
        case MapLoadBadLogVal:
            cout << "Undefined logic value in map file.\n";
            break;
        case MapLoadBadOperand:
            cout << "Used operand different then a, o, i, n or w in map file.\n";
        case MapLoadBadNoPassableVals:
            cout << "Undefined logic value in map file in no passable values list.\n";
            break;
        default:
            break;
    }
}
