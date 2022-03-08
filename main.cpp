#include "libs.hpp"
#include "Road.hpp"


int main() 
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    pthread_t thread_one;
    pthread_t thread_two;

    Road *road = new Road();
    road->draw_info();
    road->draw_speedway();


    while(1)
    {
        int is_exit = getch();

        if (is_exit == 'x') 
        {
            endwin();
            return 0;
        }
    }
}