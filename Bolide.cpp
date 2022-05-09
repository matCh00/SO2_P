#include "Bolide.hpp"

// atomic gwarantuje że nie wystąpi wyścig 
// danych i jest używany do synchronizowania 
// dostępu do pamięci między różnymi wątkami
extern atomic<bool> running_loop;

// mapy bolidów
extern map <int, tuple<int, int, char, int>> bolide1_map;
extern map <int, tuple<int, int, char, int>> bolide2_map;


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

    // dla bolidów jadących w dół
    if (type == 1)
    {
        // sprawdzamy położenie bolidów jeżdżących w kółko
        for (auto m1 : bolide1_map)
        {
            int x1 = get<0>(m1.second);
            int y1 = get<1>(m1.second);

            if ((y1 == yLoc && x1 == xLoc) || (y1 == yLoc + 1 && x1 == xLoc) || (y1 == yLoc + 2 && x1 == xLoc) || (y1 == yLoc + 3 && x1 == xLoc))
            {
                this_thread::sleep_for(chrono::milliseconds(speed * 6)); 
                break;
            } 
        }
    }
    
    yLoc++;     
}


void Bolide::mvleft()
{
    this_thread::sleep_for(chrono::milliseconds(speed));

    // dla bolidów jeżdżących w kółko
    if (type == 0)
    {
        // sprawdzamy położenie bolidów jadących w dół
        for (auto m2 : bolide2_map)
        {
            int x2 = get<0>(m2.second);
            int y2 = get<1>(m2.second);

            if ((y2 == yLoc && x2 == xLoc) || (y2 == yLoc && x2 == xLoc - 1) || (y2 == yLoc && x2 == xLoc - 2) || (y2 == yLoc && x2 == xLoc - 3))
            {
                this_thread::sleep_for(chrono::milliseconds(speed * 6)); 
                break;
            } 
        }
    }
    
    xLoc--; 
}


void Bolide::mvright()
{
    this_thread::sleep_for(chrono::milliseconds(speed));

    // dla bolidów jeżdżących w kółko
    if (type == 0)
    {
        // sprawdzamy położenie bolidów jadących w dół
        for (auto m2 : bolide2_map)
        {
            int x2 = get<0>(m2.second);
            int y2 = get<1>(m2.second);

            if ((y2 == yLoc && x2 == xLoc) || (y2 == yLoc && x2 == xLoc + 1) || (y2 == yLoc && x2 == xLoc + 2) || (y2 == yLoc && x2 == xLoc + 3))
            {
                this_thread::sleep_for(chrono::milliseconds(speed * 6)); 
                break;
            } 
        }
    }

    xLoc++;
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
        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }
}