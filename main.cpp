#include "libs.hpp"
#include "Road.hpp"
#include "Bolid.hpp"


void* movement_func(void* arg)
{
    Bolid *bolid = new Bolid(3, 3);

    for (size_t i = 0; i < 30; i++)
    {
        bolid->mvright();
        bolid->display();
        refresh();
        usleep(500*1000);
    }
    pthread_exit(0);

    return NULL;
}

int main() 
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    Road *road = new Road();
    road->draw_info();
    road->draw_speedway();

    pthread_t threads[5] = {'1', '2', '3', '4', '5'};

    for (size_t i = 0; i < 5; i++)
    {
        pthread_create(&threads[i], NULL, movement_func, NULL); 
    //}

    //for (size_t i = 0; i < 2; i++)
    //{
        pthread_join(threads[i], NULL); 
        usleep(500*1000);
    }
    
    //while(1)
    //{
        int is_exit = getch();

        if (is_exit == 'x') 
        {
            endwin();
            return 0;
        }
    //}
}