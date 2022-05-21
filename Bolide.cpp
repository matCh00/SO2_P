#include "Bolide.hpp"

// atomic gwarantuje że nie wystąpi wyścig 
// danych i jest używany do synchronizowania 
// dostępu do pamięci między różnymi wątkami
extern atomic<bool> running_loop;

// liczniki
extern atomic <unsigned int> vertical_first_count;
extern atomic <unsigned int> vertical_second_count;
extern atomic <unsigned int> horizontal_first_count;
extern atomic <unsigned int> horizontal_second_count;

// mapy bolidów
extern map <int, tuple<int, int, char, int>> bolide1_map;
extern map <int, tuple<int, int, char, int>> bolide2_map;

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
    usleep(speed * 2 * 1000);

    yLoc--;    
}


void Bolide::mvdown()
{
    usleep(speed * 2 * 1000);

    yLoc++; 
}


void Bolide::mvright()
{
    usleep(speed * 1000);

    xLoc++;
}


void Bolide::mvleft()
{
    usleep(speed * 1000);
    
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
        for (size_t x = 0; x < 90; x++)
        {
            mvright();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 1);
        }


        // SEKCJA KRYTYCZNA
        if (vertical_first_count > 0)
        {   
            // zablokowanie konkretnego wątku
            unique_lock<mutex> ul1(h1_m);
            // zmienna warunkowa czekanie na wznowienie
            h1_cv.wait(ul1, [] {return vertical_first_count == 0;});
        }
 
        horizontal_first_count++;

        // jest na skrzyżowaniu
        for (size_t x = 89; x < 100; x++)
        {
            mvright();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 1);
        }

        horizontal_first_count--;

        // powiadomienie o przejezdności
        if (horizontal_first_count == 0)
        {
            v1_cv.notify_all();
        }

        for (size_t x = 99; x < 136; x++)
        {
            mvright();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 1);
        }

        for (size_t y = 0; y < 21; y++)
        {
            mvdown();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 2);
        }

        for (size_t x = 0; x < 36; x++)
        {
            mvleft();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 3);
        }

        
        // SEKCJA KRYTYCZNA
        if (vertical_second_count > 0)
        {
            // zablokowanie konkretnego wątku
            unique_lock<mutex> ul2(h2_m);
            // zmienna warunkowa czekanie na wznowienie
            h2_cv.wait(ul2, [] {return vertical_second_count == 0;});
        }

        horizontal_second_count++;

        // jest na skrzyżowaniu
        for (size_t x = 35; x < 46; x++)
        {
            mvleft();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 3);
        }
        
        horizontal_second_count--;

        // powiadomienie o przejezdności
        if (horizontal_second_count == 0)
        {
            v2_cv.notify_all();
        }

        for (size_t x = 45; x < 136; x++)
        {
            mvleft();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 3);
        }

        for (size_t y = 0; y < 21; y++)
        {
            mvup();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 4);
        }
    }
}


void Bolide::movement_short()
{
    // trasa
    for (size_t i = 0; i < 4; i++)
    {
        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }

    vertical_first_count++;


    // SEKCJA KRYTYCZNA
    if (horizontal_first_count > 0)
    {
        // zablokowanie konkretnego wątku
        unique_lock<mutex> ul1(v1_m);
        // zmienna warunkowa czekanie na wznowienie
        v1_cv.wait(ul1, [] {return horizontal_first_count == 0;});
    }

    // jest na skrzyżownaiu
    for (size_t i = 3; i < 9; i++)
    {
        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }

    vertical_first_count--;

    // powiadomienie o przejezdności
    if (vertical_first_count == 0)
    {
        h1_cv.notify_all();
    }

    for (size_t i = 8; i < 23; i++)
    {
        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }

    vertical_second_count++;


    // SEKCJA KRYTYCZNA
    if (horizontal_second_count > 0)
    {
        // zablokowanie konkretnego wątku
        unique_lock<mutex> ul2(v2_m);
        // zmienna warunkowa czekanie na wznowienie
        v2_cv.wait(ul2, [] {return horizontal_second_count == 0;});
    }
    
    // jest na skrzyżownaiu
    for (size_t i = 22; i < 28; i++)
    {
        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }

    vertical_second_count--;

    // powiadomienie o przejezdności
    if (vertical_second_count == 0)
    {
        h2_cv.notify_all();
    }

    for (size_t i = 27; i < 31; i++)
    {
        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }
}