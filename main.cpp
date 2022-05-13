#include "libs.hpp"
#include "Road.hpp"
#include "Bolide.hpp"

#define FIRST_PAIR 1
#define SECOND_PAIR 2

// atomic gwarantuje że nie wystąpi wyścig 
// danych i jest używany do synchronizowania 
// dostępu do pamięci między różnymi wątkami
atomic<bool> running_loop(true);

// mapy bolidów
map <int, tuple<int, int, char, int>> bolide1_map;
map <int, tuple<int, int, char, int>> bolide2_map;

// wektory wątków
vector<thread> threads_1;
vector<thread> threads_2;

// zarządzanie skrzyżowaniami
atomic<bool> vertical_first_free(true);
atomic<bool> vertical_second_free(true);
atomic<bool> horizontal_first_free(true);
atomic<bool> horizontal_second_free(true);

// mutexy chronią współdzielone dane 
// przed równoczesnym dostępem wielu wątków
mutex h1_m, h2_m, v1_m, v2_m;

// zmienne warynkowe to obiekty które mogą blokować wątek 
// wywołujący dopóki nie zostanie powiadomiony o wznowieniu
condition_variable h1_cv, h2_cv, v1_cv, v2_cv;


void exit_loop()
{
    while (running_loop)
    {
        if(getch())
        {
            running_loop = false;
        }
    } 
    return;
}


bool if_horizontal_first_busy()
{
    for (int i = 112; i <= 120; i++)
    {
        if (((char)mvinch(11, i) >= 65) && ((char)mvinch(11, i) <= 90))
        {
            return true;
        }
    }
    return false;
}

bool if_horizontal_second_busy()
{
    for (int i = 112; i <= 120; i++)
    {
        if (((char)mvinch(32, i) >= 65) && ((char)mvinch(32, i) <= 90))
        {
            return true;
        }
    }
    return false;
}

bool if_vertical_first_busy()
{
    //for (int i = 8; i <= 14; i++)
    for (int i = 7; i <= 12; i++)
    {
        if (((char)mvinch(i, 116) >= 97) && ((char)mvinch(i, 116) <= 122))
        {
            return true;
        }
    }
    return false;
}

bool if_vertical_second_busy()
{
    //for (int i = 29; i <= 35; i++)
    for (int i = 28; i <= 33; i++)
    {
        if (((char)mvinch(i, 116) >= 97) && ((char)mvinch(i, 116) <= 122))
        {
            return true;
        }
    }
    return false;
}


void display_bolides()
{
    while (true)
    {
        for (auto m1 : bolide1_map)
        {
            int x1 = get<0>(m1.second);
            int y1 = get<1>(m1.second);
            char s1 = get<2>(m1.second);
            int direction = get<3>(m1.second);

            // czyszczenie poprzedniej pozycji przed kolejnym ruchem
            switch (direction)
            {
            case 1:
                if ((char)mvinch(y1, x1-1) != (char)124 && (char)mvinch(y1, x1-1) != (char)39)
                {
                    mvprintw(y1, x1-1, " ");
                }
                break;

            case 2:
                if ((char)mvinch(y1-1, x1) != (char)124 && (char)mvinch(y1-1, x1) != (char)39)
                {
                    mvprintw(y1-1, x1, " ");
                }
                break;

            case 3:
                if ((char)mvinch(y1, x1+1) != (char)124 && (char)mvinch(y1, x1+1) != (char)39)
                {
                    mvprintw(y1, x1+1, " ");
                } 
                break;

            case 4:
                if ((char)mvinch(y1+1, x1) != (char)124 && (char)mvinch(y1+1, x1) != (char)39)
                {
                    mvprintw(y1+1, x1, " ");
                }
                break;
            
            default:
                break;
            }

            // pobranie znaku z bieżącej pozycji
            char pos = (char)mvinch(y1, x1);

            // nie nadpisujemy konturów trasy
            if (pos != (char)124 && pos != (char)39)
            {
                // bolid na nowej pozycji
                attron(COLOR_PAIR(1));
                mvprintw(y1, x1, "%c", s1); 
                attroff(COLOR_PAIR(1));
            }
        }

        for (auto m2 : bolide2_map)
        {
            int x2 = get<0>(m2.second);
            int y2 = get<1>(m2.second);
            char s2 = get<2>(m2.second);
            int direction = get<3>(m2.second); 

            // czyszczenie poprzedniej pozycji przed kolejnym ruchem
            switch (direction)
            {
            case 1:
                if ((char)mvinch(y2, x2-1) != (char)124 && (char)mvinch(y2, x2-1) != (char)39)
                {
                    mvprintw(y2, x2-1, " ");
                }
                break;

            case 2:
                if ((char)mvinch(y2-1, x2) != (char)124 && (char)mvinch(y2-1, x2) != (char)39)
                {
                    mvprintw(y2-1, x2, " ");
                }
                break;

            case 3:
                if ((char)mvinch(y2, x2+1) != (char)124 && (char)mvinch(y2, x2+1) != (char)39)
                {
                    mvprintw(y2, x2+1, " ");
                }
                break;

            case 4:
                if ((char)mvinch(y2+1, x2) != (char)124 && (char)mvinch(y2+1, x2) != (char)39)
                {
                    mvprintw(y2+1, x2, " ");
                }
                break;
            
            default:
                break;
            }

            // pobranie znaku z bieżącej pozycji
            char pos = (char)mvinch(y2, x2);

            // nie nadpisujemy konturów trasy
            if (pos != (char)124 && pos != (char)39)
            {
                // bolid na nowej pozycji
                attron(COLOR_PAIR(2));
                mvprintw(y2, x2, "%c", s2); 
                attroff(COLOR_PAIR(2));
            } 
        }

        // wyczyszczenie pozycji na których kończą się wątki
        mvprintw(39, 116, " ");
        mvprintw(11, 20, " "); 

        refresh();

        
        // sprawdzenie przejezdności na skrzyżowaniach
        if (if_horizontal_first_busy() == true)
        {
            // utrzymanie objektu mutex zablokowanego
            lock_guard<mutex> lg(v1_m);
            horizontal_first_free = false;
        }
        else
        {
            horizontal_first_free = true;
            // powiadomienie zmiennej warunkowej o zdjęciu blokady
            v1_cv.notify_all();
        }
            
        if (if_horizontal_second_busy() == true)
        {
            // utrzymanie objektu mutex zablokowanego
            lock_guard<mutex> lg(v2_m);
            horizontal_second_free = false;
        }
        else
        {
            horizontal_second_free = true;
            // powiadomienie zmiennej warunkowej o zdjęciu blokady
            v2_cv.notify_all();
        }  

        if (if_vertical_first_busy() == true)
        {
            // utrzymanie objektu mutex zablokowanego
            lock_guard<mutex> lg(h1_m);
            vertical_first_free = false;
        }  
        else
        {
            vertical_first_free = true;
            // powiadomienie zmiennej warunkowej o zdjęciu blokady
            h1_cv.notify_all();
        } 

        if (if_vertical_second_busy() == true)
        {
            // utrzymanie objektu mutex zablokowanego
            lock_guard<mutex> lg(h2_m);
            vertical_second_free = false;
        } 
        else
        {
            vertical_second_free = true;
            // powiadomienie zmiennej warunkowej o zdjęciu blokady
            h2_cv.notify_all();
        }
            

        this_thread::sleep_for(chrono::milliseconds(5)); 
    }
}


void create_threads()
{
    int sign1 = 65, sign2 = 97, id = 0;

    // ciągłe tworzenie wątków
    while (running_loop)
    {    
        int speed_1 = rand() % 31 - 10;
        Bolide *bolid_1 = new Bolide(11, 15, 0, 50 + speed_1, (char)sign1++, 1, id++);
        threads_1.emplace_back([&](){bolid_1->movement_long();});

        int speed_2 = rand() % 31 - 10;
        Bolide *bolid_2 = new Bolide(4, 116, 1, 50 + speed_2, (char)sign2++, 2, id++);
        threads_2.emplace_back([&](){bolid_2->movement_short();});

        if (sign1 > 90)
        {
            sign1 = 65;
        }
        if (sign2 > 122)
        {
            sign2 = 97;
        }
        
        int delay = rand() % 600;
        usleep((1000 + delay) * 1000); 
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

    init_pair(FIRST_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(SECOND_PAIR, COLOR_GREEN, COLOR_BLACK);

    Road *road = new Road();
    road->draw_info();
    road->draw_speedway();

    
    // wątek - wyświetlanie bolidów
    thread thread_4(display_bolides);
    // detach - potrzebne do natychmiastowego kończenia wątku metodą ~thread()
    thread_4.detach();

    // wątek - koniec symulacji
    thread thread_3(exit_loop);

    // tworzenie wątków
    create_threads();
    //thread thread_5(create_threads);
    
    
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

    // natychmiastowe kończenie wątku
    // dlaczego tak? dzięki temu wyświetlamy pojazdy do momentu zakończenia wątku każdego pojazdu
    // bez tego ekran się zamraża a wątki każdego pojazdu po zakończeniu ich tworzenia kończą się bez wizualizacji
    thread_4.~thread();
    

    endwin(); // koniec ncurses

    threads_1.clear();
    threads_2.clear();
    bolide1_map.clear();
    bolide2_map.clear();

    return 0;
}