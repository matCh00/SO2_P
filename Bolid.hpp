#ifndef _BOLID_HPP_
#define _BOLID_HPP_

#include "libs.hpp"


class Bolid
{
private:
    int xLoc, yLoc, xMax, yMax;
    char character;

public:
    Bolid(int y, int x, char c);
    ~Bolid();

    int getmv();
    void mvup();
    void mvdown();
    void mvleft();
    void mvright();
    void display();
};


#endif