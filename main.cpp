#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

using namespace std;


void draw_info()
{
    mvprintw(1, 1, "SO2 Projekt - Mateusz Chalik");
    mvprintw(2, 1, "Symulacja wielowatkowa F1,  x - wyjscie");

    refresh();
}


void draw_speedway() 
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

    for (size_t j = 6; j < 29; j++)
    {
        mvprintw(j, 57, "|");
        mvprintw(j, 67, "|");
    }
    for (size_t k = 58; k < 67; k += 2)
    {
        mvprintw(8, k, "'");
        mvprintw(15, k, "'");
        mvprintw(19, k, "'");
        mvprintw(26, k, "'");
    }

    refresh();
}


void move_bolid()
{
    
}


int main() 
{
    initscr();
    noecho();
    curs_set(0);

    pthread_t thread_one;
    pthread_t thread_two;


    draw_info();
    draw_speedway();


    while(1)
    {
        int is_exit = getch();

        if (is_exit == 'x') 
        {
            endwin();
            exit(0);
        }
    }
}