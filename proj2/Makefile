CC        = clang++
CXXFLAGS  = -Os -pipe -march=native -Wall -std=c++20
LDFLAGS   =
RM        = rm -f

SRC  = $(wildcard src/*.cpp)
OBJ  = $(SRC:.cpp=.o)
EXEC = proj2

default: $(EXEC)

clean:
	$(RM) proj2 src/*.o

.cpp.o:
	$(CC) -c $(CXXFLAGS) -o $@ $<

$(EXEC): clean $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

clearscr:
	clear

run: clean clearscr $(EXEC)
	./$(EXEC)

.PHONY: all clean $(EXEC) default
