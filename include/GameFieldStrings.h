#ifndef GAMEFIELDSTRINGS_H_INCLUDED
#define GAMEFIELDSTRINGS_H_INCLUDED

#define GENERAL_HELP "Press Tab to switch betwen move and calculate move type.\n\
Press Enter to accept actually previewed move.\n\
Press F2 for information about current selected move type.\n\
Press F3 for information about map.\n\
Press F5 for logic table of not.\n\
Press F6 for logic table of and.\n\
Press F7 for logic table of or.\n\
Press F8 for logic table of implication.\n\
Press F9 for list of no passable logic values.\n\
Press F12 to quit."

#define MOVE_HELP "Use arrow keys to change direction in which you want to move.\
You can only step on tiles which value is not listed on list of no passable logic values.\
Preview of your move is marked as blue point. When you press Enter, then you will move on blue pointed position."

#define CHANGE_VALUE_HELP "Use arrow keys to choose which field on tile, occupied by you, modify.\
Two top fields are arguments for logic function placed in left bottom field.\
Arguments can be any logic value defined in logic tables.\
Logic function can be selected by typing 'a' for and, 'o' for or, 'i' for implication and 'n' for not.\
In one move you can change only one field in your tile.\
When you are satisfied in what you setup, then press Enter.\
That brings you to second step - calculation."

#define CALCULATE_HELP "Use up and down arrow key to choose if you want to change y argument in upper or bottom tile to\
result value from your tile.\
Use left and right arrow key to choose left or right tile that will have x argument changed by result value from your tile.\
Each changed tile will recursivlly calculate its value and change x and y arguments of tiles in choosen direction."


#define MOVE_ON "Move:\nUsing arrow keys change your position or change move type by pressing tab."

#define CHANGE_VALUE_ON "Change value:\nChange tile on which are you standing or press tab to move."

#define CALCULATE_ON "Calculate:\nChoose, using arrow keys, in which direction make evaluation."


#define NOT_LT "Not table:\n"
#define AND_LT "And table:\n"
#define IMPL_LT "Impl table:\n"
#define OR_LT "Or table:\n"

#define NOPASSABLES "List of no passable logic values:\n"

#define HEADER_STRING "Press F1 for help."

#define MAP_HELP "Map is parted into tiles. Each tile has four field and look like this:\n\
xy\n\
ow\n\
x and y are its arguments. They can be any logic value defined in logic tables\n\
o is an operand. It can be a which state and, o for or, i for implication and n for not.\n\
w is logic value like x and y. It is allways calculated by x o y operation. Not operation always\
set w value depends what is x.\n\
On map your position is always red marked. The goal is to reach green marked tile in fewest number of moves."

#endif // GAMEFIELDSTRINGS_H_INCLUDED
