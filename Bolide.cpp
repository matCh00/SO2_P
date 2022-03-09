#include "Bolide.hpp"


Bolide::Bolide(int y, int x, int id)
{
    xLoc = x;
    yLoc = y;
    id = id;
}

Bolide::~Bolide()
{
}

void Bolide::mvup()
{
    lastChar = mvinch(yLoc, xLoc);
    mvprintw(yLoc, xLoc, " ");
    yLoc--;
}

void Bolide::mvdown()
{
    lastChar = mvinch(yLoc, xLoc);
    mvprintw(yLoc, xLoc, " ");
    yLoc++;
}

void Bolide::mvleft()
{
    lastChar = mvinch(yLoc, xLoc);
    mvprintw(yLoc, xLoc, " ");
    xLoc--;
}

void Bolide::mvright()
{
    lastChar = mvinch(yLoc, xLoc);
    mvprintw(yLoc, xLoc, " ");
    xLoc++;
}

void Bolide::display()
{
    // TODO: zrobić coś aby zachowywać usunięte znaki
    mvprintw(yLoc, xLoc, "X");
}

void Bolide::clear()
{
    mvprintw(yLoc, xLoc, " ");
}

