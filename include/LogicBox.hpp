#ifndef LOGICBOX_HPP
#define LOGICBOX_HPP

#include <fstream>
#include "LogVal.h"
#include <unordered_map>
#include <random>
#include <vector>


using namespace std;

enum state : int{ StateOK, StateNotInitialized,
    StateFiledRead = 10,     StateTooFewArg,      StateTooManyArg,     StateTooManyDef,     StateTooFewDef,     StateNotDefVal,
    StateNotFiledRead = 20,  StateNotTooFewArg,   StateNotTooManyArg,  StateNotTooManyDef,  StateNotTooFewDef,  StateNotNotDefVal,
    StateAndFiledRead = 30,  StateAndTooFewArg,   StateAndTooManyArg,  StateAndTooManyDef,  StateAndTooFewDef,  StateAndNotDefVal,
    StateOrFiledRead = 40,   StateOrTooFewArg,    StateOrTooManyArg,   StateOrTooManyDef,   StateOrTooFewDef,   StateOrNotDefVal,
    StateImplFiledRead = 50, StateImplTooFewArg,  StateImplTooManyArg, StateImplTooManyDef, StateImplTooFewDef, StateImplNotDefVal,
    };
/* ERROR CODE LIST
StateXXXFieldRead    --  Filed to read from "XXX" file
StateXXXTooFewArg    --  Too few logic values in one of the "XXX" file lines
StateXXXTooManyArg   --  Too many logic values in one of the "XXX" file lines
StateXXXTooManyDef   --  More then one definition for one of logic values in "XXX" file
StateXXXTooFewDef    --  Not every logic value has it XXX definition
StateXXXNotDefVal    --  Using any logic value that was not defined in "not" file
*/

/*
LogicBox is set by logic tables.
Provide And, Or, Impl, Not functions to make logic operations on LogValues.
Operands have to be create by LogicBox which method is in use.
PrintLogTable return actually string.
*/

class LogicBox {
    public:
        LogicBox(fstream &notF, fstream &andF, fstream &orF, fstream &implF);
        bool SetLogVal(LogVal &a, char b);
        char GetLogVal(LogVal a);
        char GetLogVal(unsigned int a);
        LogVal CreatLogVal(char a);
        LogVal CreatRandomLogVal();
        LogVal And(LogVal a, LogVal b);
        LogVal Or(LogVal a, LogVal b);
        LogVal Impl(LogVal a, LogVal b);
        LogVal Not(LogVal a);
        string PrintLogTable(char a);
        state IsOkay();   //any code different than 0 is because constructor failed. Number represent error code
    private:
        state _correct;
        unordered_map<char,unsigned int> _logToIndex;
        unordered_map<unsigned int,char> _indexToLog;
        unsigned int _lastIndex;
        vector<unsigned int> _notTable;
        vector<vector <unsigned int> > _andTable;
        vector<vector <unsigned int> > _orTable;
        vector<vector <unsigned int> > _implTable;
        void _readNot(fstream &f);
        void _readRestFile(vector<vector <unsigned int> > &tab, fstream &f);
        void _addLogVal(char a);
        mt19937 _randGen;
};


#endif // LOGICBOX_HPP
