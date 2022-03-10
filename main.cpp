#include "libs.hpp"
#include "Road.hpp"
#include "Bolide.hpp"

const int NUM_THREADS = 4;
mutex mtx;
bool run = true;


void movement_long(int y, int x, int id, int speed)
{
    int laps = 1;

    Bolide *bolid = new Bolide(y, x, id, speed);

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

void movement_short(int y, int x, int id, int speed)
{
    Bolide *bolid = new Bolide(y, x, id, speed);

    // trasa
    for (size_t i = 0; i < 25; i++)
    {        
        mtx.lock();
        bolid->mvdown();
        bolid->display(1);
        refresh();
        mtx.unlock();
        this_thread::sleep_for(chrono::milliseconds(speed));
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
        }
    }
}

int main() 
{
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    start_color();
    
    Road *road = new Road();
    road->draw_info();
    road->draw_speedway();

    vector<thread> threads_1;
    vector<thread> threads_2;

    // wątek wyłączający symulację
    thread thread_3(exit_loop);
    thread_3.detach();

    int i = 0;

    // ciągłe tworzenie wątków
    while (run)
    {
        threads_1.emplace_back([&](){movement_long(11, 15, i, 150);});
        threads_2.emplace_back([&](){movement_short(4, 62, i, 250);});

        // konieczne aby potem wywołać destruktor ~thread()
        threads_1[i].detach();
        threads_2[i++].detach();

        usleep(1100*1000); 
    }

    // natychmiastowe zakończenie każdego wątku
    for (size_t j = 0; j < threads_1.size(); j++)
    {
        //threads_1[j].join();
        threads_1[j].~thread();
    }
    for (size_t j = 0; j < threads_2.size(); j++)
    {
        //threads_2[j].join();
        threads_2[j].~thread();
    }
    thread_3.~thread();


    system("reset");
    system("clear");
    return 0;
}