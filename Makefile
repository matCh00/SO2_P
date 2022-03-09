FILES = main.cpp Road.cpp Bolide.cpp

all:
	g++ $(FILES) -o program -lpthread -lncurses