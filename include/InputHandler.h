#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

enum InputInfo { InputExit, InputDown, InputLeft, InputRight, InputUp, InputAccept, InputToggleMoveType, InputOther, InputHelp };
enum HelpInfo { HelpMovePhase, HelpGeneral, HelpAnd, HelpOr, HelpImpl, HelpNot, HelpNoPassableValues, HelpMap};

class InputHandler
{
    public:
        InputHandler();
        InputInfo ProcessInput();
        char OtherKey;  //if not specified key was pressed it will be stored here (when InputInfo == InputOther)
        HelpInfo helpKey;   //if pressed key from help group helpKey is set (when InputInfo == InputHelp)
    private:
        int _acceptKey;
        int _upKey;
        int _downKey;
        int _leftKey;
        int _rightKey;
        int _exitKey;
        int _toggleMoveTypeKey;
        int _generalHelp;
        int _moveHelp;
        int _andHelp;
        int _orHelp;
        int _implHelp;
        int _notHelp;
        int _noPassableHelp;
        int _mapHelp;
};

#endif // INPUTHANDLER_H
