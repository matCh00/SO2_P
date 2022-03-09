#include "libs.hpp"
#include "Road.hpp"
#include "Bolide.hpp"

bool stop = false;
const int NUM_THREADS = 10;
mutex mtx;

void movement_func()
{
    int count = 0;

    Bolide *bolid = new Bolide(11, 15, 1);

    usleep(300*1000);

    for (size_t i = 0; i < 5; i++)
    {
        mtx.lock();
        bolid->mvright();
        bolid->display();
        refresh();
        mtx.unlock();
        usleep(50*1000);
    }
    while (count++ < 2)
    {
        for (size_t i = 0; i < 64; i++)
        {
            mtx.lock();
            bolid->mvright();
            bolid->display();
            refresh();
            mtx.unlock();
            usleep(50*1000);
        }
        for (size_t i = 0; i < 11; i++)
        {
            mtx.lock();
            bolid->mvdown();
            bolid->display();
            refresh();
            mtx.unlock();
            usleep(150*1000);
        }
        for (size_t i = 0; i < 64; i++)
        {
            mtx.lock();
            bolid->mvleft();
            bolid->display();
            refresh();
            mtx.unlock();
            usleep(50*1000);
        }
        for (size_t i = 0; i < 11; i++)
        {
            mtx.lock();
            bolid->mvup();
            bolid->display();
            refresh();
            mtx.unlock();
            usleep(150*1000);
        }
    }
}

int main() 
{
    initscr();
    noecho();
    curs_set(0);
    cbreak();


    Road *road = new Road();
    road->draw_info();
    road->draw_speedway();

    vector<thread> threads;

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        threads.emplace_back([&](){movement_func();});
        usleep(1500*1000); 
    }

    
    if (getchar()) 
    {
        endwin();

        system("clear");
        system("reset");
        return 0;
    }
}