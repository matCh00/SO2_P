#include "Bolide.hpp"

// atomic gwarantuje że nie wystąpi wyścig 
// danych i jest używany do synchronizowania 
// dostępu do pamięci między różnymi wątkami
extern atomic<bool> running_loop;

mutex mutx, m1, m2, m3, m4;

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

bool Bolide::check_collision(char next, char nextC, char nextC2, char nextC3, char nextC4, char nextC5)
{
    return (
    (((int)next > 96 && (int)next < 123 && (int)sign < 96) || ((int)next > 64 && (int)next < 91 && (int)sign > 96)) || 
    (((int)nextC > 96 && (int)nextC < 123 && (int)sign < 96) || ((int)nextC > 64 && (int)nextC < 91 && (int)sign > 96)) ||
    (((int)nextC2 > 96 && (int)nextC2 < 123 && (int)sign < 96) || ((int)nextC2 > 64 && (int)nextC2 < 91 && (int)sign > 96)) ||
    (((int)nextC3 > 96 && (int)nextC3 < 123 && (int)sign < 96) || ((int)nextC3 > 64 && (int)nextC3 < 91 && (int)sign > 96)) ||
    (((int)nextC4 > 96 && (int)nextC4 < 123 && (int)sign < 96) || ((int)nextC4 > 64 && (int)nextC4 < 91 && (int)sign > 96)) ||
    (((int)nextC5 > 96 && (int)nextC5 < 123 && (int)sign < 96) || ((int)nextC5 > 64 && (int)nextC5 < 91 && (int)sign > 96)));
}

void Bolide::mvup()
{
    this_thread::sleep_for(chrono::milliseconds(speed*2));

    // pobranie znaku z następnej pozycji
    m1.lock();
    char nextChar = (char)mvinch(yLoc - 1, xLoc);
    char nChar = (char)mvinch(yLoc - 2, xLoc);
    char nChar2 = (char)mvinch(yLoc - 1, xLoc + 1);
    char nChar3 = (char)mvinch(yLoc - 1, xLoc - 1);
    char nChar4 = (char)mvinch(yLoc - 2, xLoc + 1);
    char nChar5 = (char)mvinch(yLoc - 2, xLoc + 2);
    m1.unlock();

    // wykrywamy kolizje na skrzyżowaniach
    if (check_collision(nextChar, nChar, nChar2, nChar3, nChar4, nChar5))
    {
        this_thread::sleep_for(chrono::milliseconds(speed * 6)); 
    }

    yLoc--;    
}

void Bolide::mvdown()
{
    this_thread::sleep_for(chrono::milliseconds(speed*2));

    // pobranie znaku z następnej pozycji
    m2.lock();
    char nextChar = (char)mvinch(yLoc + 1, xLoc);
    char nChar = (char)mvinch(yLoc + 2, xLoc);
    char nChar2 = (char)mvinch(yLoc + 1, xLoc + 1);
    char nChar3 = (char)mvinch(yLoc + 1, xLoc - 1);
    char nChar4 = (char)mvinch(yLoc + 2, xLoc + 1);
    char nChar5 = (char)mvinch(yLoc + 2, xLoc + 2);
    m2.unlock();

    // wykrywamy kolizje na skrzyżowaniach
    if (check_collision(nextChar, nChar, nChar2, nChar3, nChar4, nChar5))
    {
        this_thread::sleep_for(chrono::milliseconds(speed * 6)); 
    }
    
    yLoc++;     
}

void Bolide::mvleft()
{
    this_thread::sleep_for(chrono::milliseconds(speed));

    // pobranie znaku z następnej pozycji
    m3.lock();
    char nextChar = (char)mvinch(yLoc, xLoc - 1);
    char nChar = (char)mvinch(yLoc, xLoc - 2);
    char nChar2 = (char)mvinch(yLoc + 1, xLoc - 1);
    char nChar3 = (char)mvinch(yLoc - 1, xLoc - 1);
    char nChar4 = (char)mvinch(yLoc + 2, xLoc - 2);
    char nChar5 = (char)mvinch(yLoc - 2, xLoc - 2);
    m3.unlock();

    // wykrywamy kolizje na skrzyżowaniach
    if (check_collision(nextChar, nChar, nChar2, nChar3, nChar4, nChar5))
    {
        this_thread::sleep_for(chrono::milliseconds(speed * 3));
    }
    
    xLoc--; 
}

void Bolide::mvright()
{
    this_thread::sleep_for(chrono::milliseconds(speed));

    // pobranie znaku z następnej pozycji
    m4.lock();
    char nextChar = (char)mvinch(yLoc, xLoc + 1);
    char nChar = (char)mvinch(yLoc, xLoc + 2);
    char nChar2 = (char)mvinch(yLoc + 1, xLoc + 1);
    char nChar3 = (char)mvinch(yLoc - 1, xLoc + 1);
    char nChar4 = (char)mvinch(yLoc + 2, xLoc + 2);
    char nChar5 = (char)mvinch(yLoc - 2, xLoc + 2);
    m4.unlock();

    // wykrywamy kolizje na skrzyżowaniach
    if (check_collision(nextChar, nChar, nChar2, nChar3, nChar4, nChar5))
    {
        this_thread::sleep_for(chrono::milliseconds(speed * 3));
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