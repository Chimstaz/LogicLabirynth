#include "LogicBox.hpp"

#include <chrono>
#include <sstream>

LogicBox::LogicBox(fstream &notF, fstream &andF, fstream &orF, fstream &implF)
    :_correct(StateOK)
    ,_lastIndex(0)
    ,_randGen(chrono::system_clock::now().time_since_epoch().count()) {

    _indexToLog.insert(make_pair(BadLogVal,' '));
    _logToIndex.insert(make_pair(' ',BadLogVal));
    if(notF.is_open()) _readNot(notF);  //readNot created list of logic values. Any value not present in "not" file is not correct
    else _correct = StateFiledRead;         //every read function set _correct. If something go wrong _correct is modified to point correctly on reason
    if(_correct != StateOK){_correct = static_cast<state>(_correct + StateNotFiledRead - StateFiledRead); return;}
    if(andF.is_open()) _readRestFile(_andTable, andF);
    else _correct = StateFiledRead;
    if(_correct != StateOK){_correct = static_cast<state>(_correct + StateAndFiledRead - StateFiledRead); return;}
    if(andF.is_open()) _readRestFile(_orTable, orF);
    else _correct = StateFiledRead;
    if(_correct != StateOK){_correct = static_cast<state>(_correct + StateOrFiledRead - StateFiledRead); return;}
    if(andF.is_open()) _readRestFile(_implTable, implF);
    else _correct = StateFiledRead;
    if(_correct != StateOK){_correct = static_cast<state>(_correct + StateImplFiledRead - StateFiledRead); return;}
}

void LogicBox::_readRestFile(vector<vector<unsigned int> > &tab, fstream &f){
    string t;
    char x, y, v;
    while(getline(f,t)){
        istringstream l(t);
        if(l >> x >> y >> v){    //if it is possibility to get three values from one line
            auto ix = _logToIndex.find(x);
            auto iy = _logToIndex.find(y);  //if any of values was not found in log to index then not default value was used
            auto iv = _logToIndex.find(v);
            if(ix == _logToIndex.end() || iy == _logToIndex.end() || iv == _logToIndex.end()) {_correct = StateNotDefVal; return;}
            if(tab[ix->second][iy->second]!=BadLogVal){_correct = StateTooManyDef; return;} //if it is second declaration of same operands in log table
            tab[ix->second][iy->second]=iv->second;
        }
        else {_correct = StateTooFewArg; return;}
        if(l >> v) {_correct = StateTooManyArg; return;}  //if there is additional value in row
    }
    for (int i = tab.size()-1; i >= 0; --i){
        for (int j = tab[i].size()-1; j >= 0; --j){
            if(tab[i][j]==BadLogVal){_correct = StateTooFewDef; return;}  //if value was not set
        }
    }
}

void LogicBox::_readNot(fstream &f){
    string t;
    char o,v;
    while(getline(f,t)){
        istringstream l(t);
        if(l >> o >> v){    //if it is possibility to get two values from one line
            _addLogVal(v);  //try to add value to dictionary
            _addLogVal(o);
            if(_notTable[_logToIndex[o]]!=BadLogVal){_correct = StateTooManyDef; return;} //second definition of same operand
            _notTable[_logToIndex[o]]=_logToIndex[v];
        }
        else {_correct = StateTooFewArg; return;}
        if(l >> o) {_correct = StateTooManyArg; return;}  //if there is additional value in row
    }
    for (int i = _notTable.size()-1; i >= 0; --i){
        if(_notTable[i]==BadLogVal){_correct = StateTooFewDef; return;}  //if value was not set
    }
}

void LogicBox::_addLogVal(char a){
    if(_logToIndex.insert(make_pair(a,_lastIndex)).second){ //insert pair return pair<iterator,bool>. If key exist second will be false.
        _indexToLog.insert(make_pair(_lastIndex,a));    //need two way dictionaries
        _lastIndex++;
        _notTable.push_back(BadLogVal);     //expend not table
        vector <unsigned int> row;
        for(unsigned int i = 1; i < _lastIndex; ++i) row.push_back(BadLogVal);   //create row with correct length
        _andTable.push_back(row);   //apply it to every two dimensional logic table
        _orTable.push_back(row);
        _implTable.push_back(row);
        for(int i = _andTable.size()-1; i >= 0; --i){   //extend every column by one additional element to keep it square
            _andTable[i].push_back(BadLogVal);
            _orTable[i].push_back(BadLogVal);
            _implTable[i].push_back(BadLogVal);
        }
    }
}

LogVal LogicBox::And(LogVal a, LogVal b){
    return LogVal(_andTable[a.GetVal()][b.GetVal()]);
}

LogVal LogicBox::Or(LogVal a, LogVal b){
    return LogVal(_orTable[a.GetVal()][b.GetVal()]);
}

LogVal LogicBox::Impl(LogVal a, LogVal b){
    return LogVal(_implTable[a.GetVal()][b.GetVal()]);
}

LogVal LogicBox::Not(LogVal a){
    return LogVal(_notTable[a.GetVal()]);
}

bool LogicBox::SetLogVal(LogVal &a, char b){
    auto i = _logToIndex.find(b);
    if(i != _logToIndex.end()){
        a = LogVal(i->second);
        return true;
    }
    else{
        a = LogVal();   //set BadLogVal
        return false;
    }
}

LogVal LogicBox::CreatLogVal(char a){
    auto i = _logToIndex.find(a);
    if(i != _logToIndex.end()){
        LogVal t(i->second);
        return t;
    }
    else{
        LogVal t;
        return t;
    }
}

LogVal LogicBox::CreatRandomLogVal(){
    int rand = (unsigned int)((((double)_randGen()+1.0)/(double)_randGen.max())*_lastIndex);
    auto i = _indexToLog.begin();
    for(; rand >0; ++i, --rand);
    LogVal t(i->first);
    return t;
}

char LogicBox::GetLogVal(LogVal a){     //printable version of LogVal
    auto i = _indexToLog.find(a.GetVal());
    if(i != _indexToLog.end()) return i->second;
    return (char)0;
}

char LogicBox::GetLogVal(unsigned int a){   //special warper if it is not necessary to create all LogVal
    auto i = _indexToLog.find(a);
    if(i != _indexToLog.end()) return i->second;
    return (char)0;
}

string LogicBox::PrintLogTable(char a){ //convert log tables to string
    vector<vector<unsigned int> > *tab;
    ostringstream stringTab;
    switch(a){
        case 'a': tab = &_andTable; break;
        case 'i': tab = &_implTable; break;
        case 'o': tab = &_orTable; break;
        case 'n':
            for(int i = _notTable.size()-1; i >= 0; --i){
                stringTab << _indexToLog[i] << " | " << _indexToLog[_notTable[i]] << "\n";
            }
        default: return stringTab.str();
    }
    for(int i = tab->size() - 1; i >= 0; --i){
        for(int j = tab->at(i).size() - 1; j >= 0; --j){
            stringTab << _indexToLog[i] << " " << _indexToLog[j] << " | " << _indexToLog[tab->at(i).at(j)] << "\n";
        }
    }
    return stringTab.str();
}

state LogicBox::IsOkay(){
    return _correct;
}

