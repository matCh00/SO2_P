#include "Road.hpp"

Road::Road()
{
}

Road::~Road()
{
}

void Road::draw_info()
{
    mvprintw(0, 1, "SO2 Projekt - Mateusz Chalik");
    mvprintw(1, 1, "Symulacja wielowatkowa F1,  keyboard input - wyjscie");
}

void Road::draw_speedway()
{
    for (size_t i = 20; i < 160; i++)
    {
        if (i % 2 == 0)
        {
            mvprintw(8, i, "*");
            mvprintw(36, i, "*");  
        }
    }
    for (size_t j = 13; j < 31; j++)
    {
        mvprintw(j, 10, "*");
        mvprintw(j, 170, "*");
    }
    for (size_t k = 0; k < 5; k++)
    {
        mvprintw(13 - k, 10 + k * 2, "*");
        mvprintw(31 + k, 10 + k * 2, "*");
        mvprintw(35 - k, 160 + k * 2, "*");
        mvprintw(8 + k, 160 + k * 2, "*");
    }

    for (size_t i = 30; i < 150; i++)
    {
        if (i % 2 == 0)
        {
            mvprintw(15, i, "*");
            mvprintw(29, i, "*");
        }   
    }
    for (size_t j = 16; j < 29; j++)
    {
        mvprintw(j, 28, "*");
        mvprintw(j, 150, "*");
    }

    for (size_t j = 5; j < 40; j++)
    {
        mvprintw(j, 111, "|");
        mvprintw(j, 121, "|");
    }
    for (size_t k = 112; k < 121; k += 2)
    {
        mvprintw(8, k, " ");
        mvprintw(15, k, " ");
        mvprintw(29, k, " ");
        mvprintw(36, k, " ");
    }
}