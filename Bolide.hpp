#ifndef _BOLIDE_HPP_
#define _BOLIDE_HPP_

#include "libs.hpp"

class Bolide
{
private:
    int xLoc, yLoc, id, speed, color;
    char sign;
    bool type;
    void mvup();
    void mvdown();
    void mvleft();
    void mvright();

public:
    Bolide(int y, int x, bool type, int speed, char sign, int color, int id);
    ~Bolide();
    void movement_long();
    void movement_short();
};


#endif