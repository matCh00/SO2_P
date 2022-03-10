#include "libs.hpp"
#include "Road.hpp"
#include "Bolide.hpp"

const int NUM_THREADS = 4;
mutex mtx;
bool run = true;

void movement_long(int y, int x, int id)
{
    int laps = 1;

    Bolide *bolid = new Bolide(y, x, id);

    // trasa
    for (size_t i = 0; i < 5; i++)
    {
        mtx.lock();
        bolid->mvright();
        bolid->display(0);
        refresh();
        mtx.unlock();
        this_thread::sleep_for(50ms);
    }
    while (laps-- > 0)
    {
        for (size_t i = 0; i < 64; i++)
        {
            mtx.lock();
            bolid->mvright();
            bolid->display(0);
            refresh();
            mtx.unlock();
            this_thread::sleep_for(50ms);
        }
        for (size_t i = 0; i < 11; i++)
        {
            mtx.lock();
            bolid->mvdown();
            bolid->display(0);
            refresh();
            mtx.unlock();
            this_thread::sleep_for(150ms);
        }
        for (size_t i = 0; i < 64; i++)
        {
            mtx.lock();
            bolid->mvleft();
            bolid->display(0);
            refresh();
            mtx.unlock();
            this_thread::sleep_for(50ms);
        }
        for (size_t i = 0; i < 11; i++)
        {
            mtx.lock();
            bolid->mvup();
            bolid->display(0);
            refresh();
            mtx.unlock();
            this_thread::sleep_for(150ms);
        }
    }

    // usunięcie znaku
    mtx.lock();
    bolid->clear();
    refresh();
    mtx.unlock();
}

void movement_short(int y, int x, int id)
{
    Bolide *bolid = new Bolide(y, x, id);

    // trasa
    for (size_t i = 0; i < 23; i++)
    {
        mtx.lock();
        bolid->mvdown();
        bolid->display(1);
        refresh();
        mtx.unlock();
        this_thread::sleep_for(250ms);
    }
    
    // usunięcie znaku
    mtx.lock();
    bolid->clear();
    refresh();
    mtx.unlock();
}

void exit_loop()
{
    while (true)
    {
        if (getchar()) 
        {
            run = false;
            return;
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


    vector<thread> threads_1;
    vector<thread> threads_2;

    //thread thread_3(exit_loop);

    int i = 0;

    // tworzenie wątków
    //while (run)
    while (i < NUM_THREADS)
    {
        threads_1.emplace_back([&](){movement_long(11, 15, i);});
        threads_2.emplace_back([&](){movement_short(5, 62, i++);});
        //threads_1[i].detach();
        //threads_2[i++].detach();
        usleep(1500*1000); 
    }
    for (size_t j = 0; j < i; j++)
    {
        threads_1[j].join();
        threads_2[j].join();

        //threads_1[i].thread::~thread();
        //threads_2[i].thread::~thread();
    }
    
    endwin();
    system("clear");
    system("reset");
    return 0;
}