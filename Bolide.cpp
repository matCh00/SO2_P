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

void Bolide::display(int type)
{
    // TODO: zrobić coś aby zachowywać usunięte znaki
    if (type == 0)
    {
        mvprintw(yLoc, xLoc, "X");
    }
    else
    {
        mvprintw(yLoc, xLoc, "O");
    }
}

void Bolide::clear()
{
    mvprintw(yLoc, xLoc, " ");
}

