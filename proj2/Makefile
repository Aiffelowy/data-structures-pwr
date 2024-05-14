CC        = clang++
CXXFLAGS  = -O3 -pipe -march=native -Wall -std=c++20
LDFLAGS   =
RM        = rm -f

THREADS = $(shell nproc)

SRC  = $(wildcard src/*.cpp src/*/*/*.cpp)
OBJ  = $(SRC:.cpp=.o)
EXEC = menu

SRC_PLUGINS = $(wildcard plugins/*.cpp)
OBJ_PLUGINS = $(SRC_PLUGINS:.cpp=.o)
SO_PLUGINS = $(OBJ_PLUGINS:.o=.so)

default: MAKE

clean:
	$(RM) $(EXEC) $(OBJ)

clean_plugins:
	$(RM) $(OBJ_PLUGINS) $(SO_PLUGINS)

.cpp.o:
	$(CC) -c $(CXXFLAGS) -o $@ $<

$(EXEC): clean $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

$(OBJ_PLUGINS):
	$(CC) -c $(CXXFLAGS) -fPIC -o $@ $(@:.o=.cpp)

$(SO_PLUGINS): $(OBJ_PLUGINS)
	$(CC) -shared $(LDFLAGS) -o $@ $(@:.so=.o)

MAKE: clean clean_plugins
	$(MAKE) -j$(THREADS) all

all: $(EXEC) $(SO_PLUGINS)

clearscr:
	clear

run: clean clearscr $(EXEC)
	./$(EXEC)

.PHONY: all clean $(EXEC) default
