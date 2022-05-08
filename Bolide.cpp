#include "Bolide.hpp"

// atomic gwarantuje że nie wystąpi wyścig 
// danych i jest używany do synchronizowania 
// dostępu do pamięci między różnymi wątkami
extern atomic<bool> running_loop;

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
    terminate();
}

bool Bolide::check_collision(char nextChar, char nextNextChar)
{
    return ((((int)nextChar > 96 && (int)nextChar < 123 && (int)sign < 96) || ((int)nextChar > 64 && (int)nextChar < 91 && (int)sign > 96)) || 
    (((int)nextNextChar > 96 && (int)nextNextChar < 123 && (int)sign < 96) || ((int)nextNextChar > 64 && (int)nextNextChar < 91 && (int)sign > 96)));
}

void Bolide::mvup()
{
    this_thread::sleep_for(chrono::milliseconds(speed*2));

    // pobranie znaku z następnej pozycji
    char nextChar = (char)mvinch(yLoc - 1, xLoc);
    char nextNextChar = (char)mvinch(yLoc - 2, xLoc);

    // wykrywamy kolizje na skrzyżowaniach
    if (check_collision(nextChar, nextNextChar))
    {
        this_thread::sleep_for(chrono::milliseconds(speed * 4)); 
    }

    yLoc--;    
}

void Bolide::mvdown()
{
    this_thread::sleep_for(chrono::milliseconds(speed*2));

    // pobranie znaku z następnej pozycji
    char nextChar = (char)mvinch(yLoc + 1, xLoc);
    char nextNextChar = (char)mvinch(yLoc + 2, xLoc);

    // wykrywamy kolizje na skrzyżowaniach
    if (check_collision(nextChar, nextNextChar))
    {
        this_thread::sleep_for(chrono::milliseconds(speed * 4)); 
    }
    
    yLoc++;     
}

void Bolide::mvleft()
{
    this_thread::sleep_for(chrono::milliseconds(speed));

    // pobranie znaku z następnej pozycji
    char nextChar = (char)mvinch(yLoc, xLoc - 1);
    char nextNextChar = (char)mvinch(yLoc, xLoc - 2);

    // wykrywamy kolizje na skrzyżowaniach
    if (check_collision(nextChar, nextNextChar))
    {
        this_thread::sleep_for(chrono::milliseconds(speed * 2));
    }
    
    xLoc--; 
}

void Bolide::mvright()
{
    this_thread::sleep_for(chrono::milliseconds(speed));

    // pobranie znaku z następnej pozycji
    char nextChar = (char)mvinch(yLoc, xLoc + 1);
    char nextNextChar = (char)mvinch(yLoc, xLoc + 2);

    // wykrywamy kolizje na skrzyżowaniach
    if (check_collision(nextChar, nextNextChar))
    {
        this_thread::sleep_for(chrono::milliseconds(speed * 2));
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