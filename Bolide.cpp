#include "Bolide.hpp"

#define FIRST_PAIR 1
#define SECOND_PAIR 2

Bolide::Bolide(int y, int x, int id)
{
    xLoc = x;
    yLoc = y;
    id = id;
    init_pair(FIRST_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(SECOND_PAIR, COLOR_GREEN, COLOR_BLACK);
}

Bolide::~Bolide()
{
    terminate();
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
        attron(COLOR_PAIR(FIRST_PAIR));
        // nie nadpisujemy konturów trasy
        if (lastChar != (char)124 && lastChar != (char)39)
        {
            mvprintw(yLoc, xLoc, "X");
        }
        attroff(COLOR_PAIR(FIRST_PAIR));
    }
    else
    {
        attron(COLOR_PAIR(SECOND_PAIR));
        // nie nadpisujemy konturów trasy
        if (lastChar != (char)124 && lastChar != (char)39)
        {
            mvprintw(yLoc, xLoc, "O");
        }
        attroff(COLOR_PAIR(SECOND_PAIR));
    }
}

void Bolide::clear()
{
    mvprintw(yLoc, xLoc, " ");
}