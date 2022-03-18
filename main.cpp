#include "libs.hpp"
#include "Road.hpp"
#include "Bolide.hpp"

bool run = true;

void exit_loop()
{
    while (true)
    {
        if(getchar())
        {
            run = false;
            return;
        }
    } 
}

int main() 
{
    // inicjalizacja
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    start_color();

    srand (time(NULL));

    Road *road = new Road();
    road->draw_info();
    road->draw_speedway();

    vector<thread> threads_1;
    vector<thread> threads_2;

    // wątek wyłączający symulację
    thread thread_3(exit_loop);
    //thread_3.detach();

    int i = 0, char1 = 65, char2 = 97;

    // ciągłe tworzenie wątków
    while (run)
    {        
        int speed_1 = rand() % 41 - 20;
        Bolide *bolid_1 = new Bolide(11, 15, i, 40 + speed_1, (char)char1, 1);
        threads_1.emplace_back([&](){bolid_1->movement_long();});

        int speed_2 = rand() % 21 - 10;
        Bolide *bolid_2 = new Bolide(4, 62, i, 50 + speed_2, (char)char2, 2);
        threads_2.emplace_back([&](){bolid_2->movement_short();});

        // konieczne aby potem wywołać destruktor ~thread()
        threads_1[i].detach();
        threads_2[i++].detach();

        char1++;
        if (char1 > 90)
        {
            char1 = 65;
        }
        char2++;
        if (char2 > 122)
        {
            char2 = 97;
        }
        
        usleep(1000*1000); 
    }

    // natychmiastowe zakończenie każdego wątku
    for (size_t j = 0; j < threads_1.size(); j++)
    {
        //threads_1[j].join();  //thread.cancel(), każdy bolid inna litera, lepszy join()
        threads_1[j].~thread();  //bez tego
    }
    for (size_t j = 0; j < threads_2.size(); j++)
    {
        //threads_2[j].join();
        threads_2[j].~thread();
    }
    thread_3.join();
    //thread_3.~thread();

    endwin();             // koniec ncurses
    system("stty sane");  // naprawia bug: po zakończeniu programu znaki w terminalu były niewidzialne
    system("clear");      // wyczyszczenie terminala
    return 0;
}