IDIR =include
CXX=g++
CXXFLAGS=-O3 -std=c++11 -I$(IDIR)

ODIR=obj
CDIR=src

LIBS=-lncurses

_DEPS = ColorsDef.h Game.h GameField.h GameFieldStrings.h GlobalDefines.h InputHandler.h LogicBox.hpp LogVal.h Map.h MessageWindow.h Sleep.h Tile.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = ColorDefiner.o Game.o GameField.o InputHandler.o LogicBox.o LogVal.o Map.o MessageWindow.o Tile.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(CDIR)/%.cpp $(DEPS)
	@mkdir -p $(ODIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

logic: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
