#ifndef _BOLID_HPP_
#define _BOLID_HPP_

#include "libs.hpp"


class Bolid
{
private:
    int xLoc, yLoc, xMax, yMax, id;
    char lastChar;

public:
    Bolid(int y, int x, int id);
    ~Bolid();

    void mvup();
    void mvdown();
    void mvleft();
    void mvright();
    void display();
};


#endif