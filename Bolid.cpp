#include "Bolid.hpp"


Bolid::Bolid(int y, int x, int id)
{
    xLoc = x;
    yLoc = y;
    id = id;
}

Bolid::~Bolid()
{
}

void Bolid::mvup()
{
    lastChar = mvinch(yLoc, xLoc);
    mvprintw(yLoc, xLoc, " ");
    yLoc--;
}

void Bolid::mvdown()
{
    lastChar = mvinch(yLoc, xLoc);
    mvprintw(yLoc, xLoc, " ");
    yLoc++;
}

void Bolid::mvleft()
{
    lastChar = mvinch(yLoc, xLoc);
    mvprintw(yLoc, xLoc, " ");
    xLoc--;
}

void Bolid::mvright()
{
    lastChar = mvinch(yLoc, xLoc);
    mvprintw(yLoc, xLoc, " ");
    xLoc++;
}

void Bolid::display()
{
    // TODO: zrobić coś aby zachowywać usunięte znaki
    mvprintw(yLoc, xLoc, "X");
}

