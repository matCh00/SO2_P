#include "libs.hpp"
#include "Road.hpp"
#include "Bolide.hpp"

const int NUM_THREADS = 10;
mutex mtx;
bool stop = false;

void movement_long(int y, int x, int id)
{
    int count = 0;

    Bolide *bolid = new Bolide(y, x, id);

    this_thread::sleep_for(300ms);

    for (size_t i = 0; i < 5; i++)
    {
        mtx.lock();
        bolid->mvright();
        bolid->display();
        refresh();
        mtx.unlock();
        this_thread::sleep_for(50ms);
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
            this_thread::sleep_for(50ms);
        }
        for (size_t i = 0; i < 11; i++)
        {
            mtx.lock();
            bolid->mvdown();
            bolid->display();
            refresh();
            mtx.unlock();
            this_thread::sleep_for(150ms);
        }
        for (size_t i = 0; i < 64; i++)
        {
            mtx.lock();
            bolid->mvleft();
            bolid->display();
            refresh();
            mtx.unlock();
            this_thread::sleep_for(50ms);
        }
        for (size_t i = 0; i < 11; i++)
        {
            mtx.lock();
            bolid->mvup();
            bolid->display();
            refresh();
            mtx.unlock();
            this_thread::sleep_for(150ms);
        }
    }

    mtx.lock();
    bolid->clear();
    refresh();
    mtx.unlock();
}

void movement_short(int y, int x, int id)
{
    int count = 0;

    Bolide *bolid = new Bolide(y, x, id);

    this_thread::sleep_for(200ms);

    for (size_t i = 0; i < 23; i++)
    {
        mtx.lock();
        bolid->mvdown();
        bolid->display();
        refresh();
        mtx.unlock();
        this_thread::sleep_for(90ms);
    }
    
    mtx.lock();
    bolid->clear();
    refresh();
    mtx.unlock();
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


    vector<thread> threads_1;
    vector<thread> threads_2;

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        threads_1.emplace_back([&](){movement_long(11, 15, i);});
        threads_2.emplace_back([&](){movement_short(5, 62, i);});
        usleep(1500*1000); 
    }
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        threads_1[i].join();
        threads_2[i].join();
    }

    
    if (getchar()) 
    {
        endwin();

        system("clear");
        system("reset");
        return 0;
    }
}