#ifndef _BOLIDE_HPP_
#define _BOLIDE_HPP_

#include "libs.hpp"


class Bolide
{
private:
    int xLoc, yLoc, xMax, yMax, id, speed;
    void dont_override();

public:
    Bolide(int y, int x, int id, int speed);
    ~Bolide();

    void mvup();
    void mvdown();
    void mvleft();
    void mvright();
    void display(int type);
    void clear(); 
};


#endif