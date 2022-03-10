#ifndef _BOLIDE_HPP_
#define _BOLIDE_HPP_

#include "libs.hpp"

class Bolide
{
private:
    int xLoc, yLoc, xMax, yMax, id, speed, color;
    char type;
    void dont_override();

public:
    Bolide(int y, int x, int id, int speed, char type, int color);
    ~Bolide();

    void mvup();
    void mvdown();
    void mvleft();
    void mvright();
    void display();
    void clear(); 
};


#endif