#include "libs.hpp"
#include "Road.hpp"
#include "Bolide.hpp"

const int NUM_THREADS = 4;
//mutex mtx;
bool run = true;


void movement_long(int y, int x, int id, int speed, char type, int color)
{
    Bolide *bolid = new Bolide(y, x, id, speed, type, color);

    int laps = 1;

    // trasa
    for (size_t i = 0; i < 5; i++)
    {
        bolid->mvright();
    }
    while (laps-- > 0)
    {
        for (size_t i = 0; i < 64; i++)
        {
            bolid->mvright();
        }
        for (size_t i = 0; i < 11; i++)
        {
            bolid->mvdown();
        }
        for (size_t i = 0; i < 64; i++)
        {
            bolid->mvleft();
        }
        for (size_t i = 0; i < 11; i++)
        {
            bolid->mvup();
        }
    }

    // usunięcie znaku
    bolid->clear();
}

void movement_short(int y, int x, int id, int speed, char type, int color)
{
    Bolide *bolid = new Bolide(y, x, id, speed, type, color);

    // trasa
    for (size_t i = 0; i < 25; i++)
    {        
        bolid->mvdown();
    }
    
    // usunięcie znaku
    bolid->clear();
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
        threads_1.emplace_back([&](){movement_long(11, 15, i, 40, 'X', 1);});
        threads_2.emplace_back([&](){movement_short(4, 62, i, 70, 'O', 2);});

        // konieczne aby potem wywołać destruktor ~thread()
        threads_1[i].detach();
        threads_2[i++].detach();

        usleep(1000*1000); 
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