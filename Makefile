FILES = main.cpp Road.cpp Bolid.cpp

all:
	g++ $(FILES) -o program -pthread -lncurses