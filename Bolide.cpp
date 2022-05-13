#include "Bolide.hpp"

// atomic gwarantuje że nie wystąpi wyścig 
// danych i jest używany do synchronizowania 
// dostępu do pamięci między różnymi wątkami
extern atomic<bool> running_loop;

// mapy bolidów
extern map <int, tuple<int, int, char, int>> bolide1_map;
extern map <int, tuple<int, int, char, int>> bolide2_map;

// zarządzanie skrzyżowaniami
extern atomic<bool> vertical_first_free;
extern atomic<bool> vertical_second_free;
extern atomic<bool> horizontal_first_free;
extern atomic<bool> horizontal_second_free;

// mutexy chronią współdzielone dane 
// przed równoczesnym dostępem wielu wątków
extern mutex h1_m, h2_m, v1_m, v2_m;

// zmienne warynkowe to obiekty które mogą blokować wątek 
// wywołujący dopóki nie zostanie powiadomiony o wznowieniu
extern condition_variable h1_cv, h2_cv, v1_cv, v2_cv;


Bolide::Bolide(int y, int x, bool type, int speed, char sign, int color, int id)
{
    this->xLoc = x;
    this->yLoc = y;
    this->type = type;
    this->speed = speed;
    this->sign = sign;
    this->color = color;
    this->id = id;
}


Bolide::~Bolide()
{
}


void Bolide::mvup()
{
    this_thread::sleep_for(chrono::milliseconds(speed*2));

    yLoc--;    
}


void Bolide::mvdown()
{
    this_thread::sleep_for(chrono::milliseconds(speed*2));

    yLoc++; 
}


void Bolide::mvright()
{
    this_thread::sleep_for(chrono::milliseconds(speed));

    xLoc++;
}


void Bolide::mvleft()
{
    this_thread::sleep_for(chrono::milliseconds(speed));
    
    xLoc--; 
}


void Bolide::movement_long()
{
    // trasa
    for (size_t i = 0; i < 5; i++)
    {
        mvright();
        bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 1);
    }

    while (running_loop)  // nieskończona liczba okrążeń - globalna zmienna atomic
    {
        for (size_t i = 0; i < 136; i++)
        {
            // zatrzymuje się przed pierwszym skrzyżowaniem
            if (xLoc == 110)
            {
                // SEKCJA KRYTYCZNA

                // zablokowanie konkretnego wątku
                unique_lock<mutex> ul(h1_m);
                // zmienna warunkowa czekanie na wznowienie
                h1_cv.wait(ul, [] {return vertical_first_free == true;});
            }

            mvright();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 1);
        }
        for (size_t i = 0; i < 21; i++)
        {
            mvdown();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 2);
        }
        for (size_t i = 0; i < 136; i++)
        {
            // zatrzymuje się przed drugim skrzyżowaniem
            if (xLoc == 122)
            {
                // SEKCJA KRYTYCZNA

                // zablokowanie konkretnego wątku
                unique_lock<mutex> ul(h2_m);
                // zmienna warunkowa czekanie na wznowienie
                h2_cv.wait(ul, [] {return vertical_second_free == true;});
            }

            mvleft();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 3);
        }
        for (size_t i = 0; i < 21; i++)
        {
            mvup();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 4);
        }
    }
}


void Bolide::movement_short()
{
    // trasa
    for (size_t i = 0; i < 35; i++)
    {
        // dla bolidów jadących w dół
        if (type == 1)
        {
            // zatrzymuje się przed pierwszym skrzyżowaniem
            if (yLoc == 8)
            {
                // SEKCJA KRYTYCZNA

                // zablokowanie konkretnego wątku
                unique_lock<mutex> ul(v1_m);
                // zmienna warunkowa czekanie na wznowienie
                v1_cv.wait(ul, [] {return horizontal_first_free == true;});
            }

            // zatrzymuje się przed drugim skrzyżowaniem
            else if (yLoc == 29)
            {
                // SEKCJA KRYTYCZNA

                // zablokowanie konkretnego wątku
                unique_lock<mutex> ul(v2_m);
                // zmienna warunkowa czekanie na wznowienie
                v2_cv.wait(ul, [] {return horizontal_second_free == true;});
            }
        }

        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }
}