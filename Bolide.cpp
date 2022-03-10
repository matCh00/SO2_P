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

void Bolide::dont_override()
{
    // nie usuwamy konturów po przejechaniu bolidu
    if ((char)mvinch(yLoc, xLoc) != (char)124 && (char)mvinch(yLoc, xLoc) != (char)39)
    {
        mvprintw(yLoc, xLoc, " ");
    }
}

void Bolide::mvup()
{
    dont_override();
    yLoc--;
}

void Bolide::mvdown()
{
    dont_override();
    yLoc++;
}

void Bolide::mvleft()
{
    dont_override();
    xLoc--;
}

void Bolide::mvright()
{
    dont_override();
    xLoc++; 
}

void Bolide::display(int type)
{
    // pobranie znaku z bieżącej pozycji
    char lastChar = (char)mvinch(yLoc, xLoc);

    if (type == 0)
    {
        // nie nadpisujemy konturów trasy
        if (lastChar != (char)124 && lastChar != (char)39)
        {
            mvprintw(yLoc, xLoc, "X");
        }
    }
    else
    {
        // nie nadpisujemy konturów trasy
        if (lastChar != (char)124 && lastChar != (char)39)
        {
            mvprintw(yLoc, xLoc, "O");
        }
    }
}

void Bolide::clear()
{
    mvprintw(yLoc, xLoc, " ");
}