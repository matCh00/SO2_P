#include "libs.hpp"
#include "Road.hpp"
#include "Bolide.hpp"

// atomic gwarantuje że nie wystąpi wyścig 
// danych i jest używany do synchronizowania 
// dostępu do pamięci między różnymi wątkami
atomic<bool> running_loop(true);

void exit_loop()
{
    while (true)
    {
        if(getchar())
        {
            running_loop = false;
            return;
        }
    } 
}

int main() 
{
    initscr(); // inicjalizacja
    noecho();  // input nie będzie wyświetlany na ekranie
    cbreak();  // klawisze są zwracane od razu
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

    int id = 0, char1 = 65, char2 = 97;

    // ciągłe tworzenie wątków
    while (running_loop)
    {        
        int speed_1 = rand() % 41 - 20;
        Bolide *bolid_1 = new Bolide(11, 15, id, 40 + speed_1, (char)char1++, 1);
        threads_1.emplace_back([&](){bolid_1->movement_long();});

        int speed_2 = rand() % 21 - 10;
        Bolide *bolid_2 = new Bolide(4, 62, id++, 50 + speed_2, (char)char2++, 2);
        threads_2.emplace_back([&](){bolid_2->movement_short();});

        if (char1 > 90)
        {
            char1 = 65;
        }
        if (char2 > 122)
        {
            char2 = 97;
        }
        
        usleep(1000*1000); 
    }

    // bezpieczne zakończenie każdego wątku
    for (size_t j = 0; j < threads_1.size(); j++)
    {
        threads_1[j].join();
    }
    for (size_t j = 0; j < threads_2.size(); j++)
    {
        threads_2[j].join();
    }
    thread_3.join();


    endwin(); // koniec ncurses
    return 0;
}