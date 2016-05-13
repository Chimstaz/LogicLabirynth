# LogicLabirynth
Logic game


# Compile
On linux: Run make and should be done. This will use precompiled verison of ncurses library. If you have installed package libncurses5-dev you can set LIBS in makefile to -lncurses and delete lib directory to use already installed version of ncurses. 

On Windows: Download PDCurses and compile it. Change in file include\GlobalDefines.h CURSESPATH to location of your curses.h in downloaded PDCurses. In makefile change LIBS to path to your pdcurses.a.

# Instruction
Run ./loigic -h after compilation. It will show help. In game itself is also help.

# First part of 3rd task
In directory czesc1zadania3 there are two programs that have one big condition block. There is also bash script to test if they are working correctly.
