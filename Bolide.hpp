#ifndef _BOLIDE_HPP_
#define _BOLIDE_HPP_

#include "libs.hpp"

class Bolide
{
private:
    int xLoc, yLoc, id, speed, color;
    char sign;
    bool type;
    void remove_trace();
    void detect_collision_down();
    void detect_collision_right();
    void detect_collision_left();
    void mvup();
    void mvdown();
    void mvleft();
    void mvright();
    void display();
    void clear(); 

public:
    Bolide(int y, int x, bool type, int speed, char sign, int color);
    ~Bolide();
    void movement_long();
    void movement_short();
    int getX();
    int getY();
};


#endif