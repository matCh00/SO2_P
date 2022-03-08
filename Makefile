FILES = main.cpp Road.cpp

all:
	g++ $(FILES) -o program -pthread -lncurses