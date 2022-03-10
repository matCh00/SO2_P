#include "Bolide.hpp"

#define FIRST_PAIR 1
#define SECOND_PAIR 2
mutex mut;

Bolide::Bolide(int y, int x, int id, int speed, char type, int color)
{
    this->xLoc = x;
    this->yLoc = y;
    this->id = id;
    this->speed = speed;
    this->type = type;
    this->color = color;
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
    this_thread::sleep_for(chrono::milliseconds(speed*2));
    mut.lock();
    dont_override();
    yLoc--;
    display();
}

void Bolide::mvdown()
{
    this_thread::sleep_for(chrono::milliseconds(speed*2));
    mut.lock();
    dont_override();
    yLoc++;
    display();
}

void Bolide::mvleft()
{
    mut.lock();
    dont_override();
    xLoc--;
    display();
}

void Bolide::mvright()
{
    mut.lock();
    dont_override();
    xLoc++; 
    display();
}

void Bolide::clear()
{
    mut.lock();
    mvprintw(yLoc, xLoc, " ");
    refresh();
    mut.unlock();
}

void Bolide::display()
{
    // pobranie znaku z bieżącej pozycji
    char lastChar = (char)mvinch(yLoc, xLoc);

    if (type == 'X')
    {
        attron(COLOR_PAIR(color));
        // nie nadpisujemy konturów trasy
        if (lastChar != (char)124 && lastChar != (char)39)
        {
            mvprintw(yLoc, xLoc, "%c", type);
        }
        attroff(COLOR_PAIR(color));
    }
    else
    {
        attron(COLOR_PAIR(color));
        // nie nadpisujemy konturów trasy
        if (lastChar != (char)124 && lastChar != (char)39)
        {
            mvprintw(yLoc, xLoc, "%c", type);
        }
        attroff(COLOR_PAIR(color));
    }
    refresh();
    mut.unlock();
    this_thread::sleep_for(chrono::milliseconds(speed));
}

void Bolide::movement_long()
{
    int laps = 1;

    // trasa
    for (size_t i = 0; i < 5; i++)
    {
        mvright();
    }
    while (laps-- > 0)
    {
        for (size_t i = 0; i < 64; i++)
        {
            mvright();
        }
        for (size_t i = 0; i < 11; i++)
        {
            mvdown();
        }
        for (size_t i = 0; i < 64; i++)
        {
            mvleft();
        }
        for (size_t i = 0; i < 11; i++)
        {
            mvup();
        }
    }

    // usunięcie znaku
    clear();
}

void Bolide::movement_short()
{
    // trasa
    for (size_t i = 0; i < 25; i++)
    {        
        mvdown();
    }
    
    // usunięcie znaku
    clear();
}