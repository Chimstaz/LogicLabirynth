#ifndef LOGVAL_H
#define LOGVAL_H



const unsigned int BadLogVal = (~0);

class LogVal {
    public:
        LogVal() : _val(BadLogVal) { };     //if no value is specified then BadLogVal is set
        LogVal(unsigned int a) : _val(a) { };   //should be used only by LogicBox

        //operator= is automatic generated and this one is good for now

        bool operator== (LogVal &a){
            return a.GetVal() == _val;
        };

        bool operator!= (LogVal &a){
            return a.GetVal() != _val;
        };

        bool IsSet(){
            return _val != BadLogVal;   //every value different then BadLogVal return true, but it can be different in different LogicBoxes
        }

        unsigned int GetVal(){
            return _val;
        };

    private:
        unsigned int _val;
};

#endif // LOGVAL_H
