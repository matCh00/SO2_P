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
    mvprintw(1, 1, "Symulacja wielowatkowa F1,  getchar() - wyjscie");
}

void Road::draw_speedway()
{
    for (size_t i = 20; i < 86; i++)
    {
        if (i % 2 == 0)
        {
            mvprintw(8, i, "*");

            if (i < 85)
            {
                mvprintw(26, i, "*");
            }  
        }
    }
    for (size_t j = 13; j < 21; j++)
    {
        mvprintw(j, 10, "*");
        mvprintw(j, 96, "*");
    }
    for (size_t k = 0; k < 5; k++)
    {
        mvprintw(13 - k, 10 + k * 2, "*");
        mvprintw(21 + k, 10 + k * 2, "*");
        mvprintw(25 - k, 86 + k * 2, "*");
        mvprintw(8 + k, 86 + k * 2, "*");
    }

    for (size_t i = 30; i < 76; i++)
    {
        if (i % 2 == 0)
        {
            mvprintw(15, i, "*");
            mvprintw(19, i, "*");
        }   
    }
    for (size_t j = 16; j < 19; j++)
    {
        mvprintw(j, 28, "*");
        mvprintw(j, 76, "*");
    }

    for (size_t j = 5; j < 30; j++)
    {
        mvprintw(j, 57, "|");
        mvprintw(j, 67, "|");
    }
    for (size_t k = 58; k < 67; k += 2)
    {
        mvprintw(8, k, "'");
        mvprintw(15, k, " ");
        mvprintw(19, k, " ");
        mvprintw(26, k, "'");
    }
}