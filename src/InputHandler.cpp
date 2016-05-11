#include "InputHandler.h"

#include "GlobalDefines.h"
#include CURSESPATH

InputHandler::InputHandler()
    :_acceptKey(10) //enter_key
    ,_upKey(KEY_UP)
    ,_downKey(KEY_DOWN)
    ,_leftKey(KEY_LEFT)
    ,_rightKey(KEY_RIGHT)
    ,_exitKey(KEY_F(12))
    ,_toggleMoveTypeKey(9) //tab_key
    ,_generalHelp(KEY_F(1))
    ,_moveHelp(KEY_F(2))
    ,_andHelp(KEY_F(6))
    ,_orHelp(KEY_F(7))
    ,_implHelp(KEY_F(8))
    ,_notHelp(KEY_F(5))
    ,_noPassableHelp(KEY_F(9))
    ,_mapHelp(KEY_F(3))
     { }

InputInfo InputHandler::ProcessInput(){
    int key = getch();
    if (key <= 255) {   //set OtherKey with code of last character
        OtherKey = (char)key;
    }
    else {
        OtherKey = 255;
    }
    //list of defined keys
    if (key == _acceptKey) return InputAccept;
    if (key == _upKey    ) return InputUp;
    if (key == _downKey  ) return InputDown;
    if (key == _leftKey  ) return InputLeft;
    if (key == _rightKey ) return InputRight;
    if (key == _exitKey  ) return InputExit;
    if (key == _toggleMoveTypeKey) return InputToggleMoveType;
    if (key == _generalHelp) {helpKey = HelpGeneral;   return InputHelp;}
    if (key == _moveHelp   ) {helpKey = HelpMovePhase; return InputHelp;}
    if (key == _andHelp    ) {helpKey = HelpAnd;       return InputHelp;}
    if (key == _orHelp     ) {helpKey = HelpOr;        return InputHelp;}
    if (key == _implHelp   ) {helpKey = HelpImpl;      return InputHelp;}
    if (key == _notHelp    ) {helpKey = HelpNot;       return InputHelp;}
    if (key == _noPassableHelp) {helpKey = HelpNoPassableValues; return InputHelp;}
    if (key == _mapHelp    ) {helpKey = HelpMap;       return InputHelp;}
    return InputOther;
}
