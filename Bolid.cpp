#include "Bolid.hpp"


Bolid::Bolid(int y, int x)
{
    xLoc = x;
    yLoc = y;
}

Bolid::~Bolid()
{
}

void Bolid::mvup()
{
    mvprintw(yLoc, xLoc, " ");
    yLoc++;
}

void Bolid::mvdown()
{
    mvprintw(yLoc, xLoc, " ");
    yLoc++;
}

void Bolid::mvleft()
{
    mvprintw(yLoc, xLoc, " ");
    xLoc--;
}

void Bolid::mvright()
{
    mvprintw(yLoc, xLoc, " ");
    xLoc++;
}

void Bolid::display()
{
    mvprintw(yLoc, xLoc, "O");
}

