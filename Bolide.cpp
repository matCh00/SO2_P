#include "Bolide.hpp"

#define FIRST_PAIR 1
#define SECOND_PAIR 2

// mutexy chronią współdzielone dane 
// przed równoczesnym dostępem wielu wątków
mutex mut, mtx;

// atomic gwarantuje że nie wystąpi wyścig 
// danych i jest używany do synchronizowania 
// dostępu do pamięci między różnymi wątkami
extern atomic<bool> running_loop;
//atomic<bool> no_collision;

Bolide::Bolide(int y, int x, bool type, int speed, char sign, int color)
{
    this->xLoc = x;
    this->yLoc = y;
    this->type = type;
    this->speed = speed;
    this->sign = sign;
    this->color = color;
    init_pair(FIRST_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(SECOND_PAIR, COLOR_GREEN, COLOR_BLACK);
}

Bolide::~Bolide()
{
    terminate();
}

void Bolide::remove_trace()
{
    // nie usuwamy konturów po przejechaniu bolidu
    if ((char)mvinch(yLoc, xLoc) != (char)124 && (char)mvinch(yLoc, xLoc) != (char)39)
    {
        mvprintw(yLoc, xLoc, " ");
    }
}

void Bolide::detect_collision_down()
{
    //lock_guard<mutex> lock(mtx);
    while (true)
    {
        if (!((char)mvinch(yLoc + 1, xLoc) >= (char)65) && !((char)mvinch(yLoc, xLoc) <= (char)90))
        {
            break;
            return;
        }
    }
}

void Bolide::detect_collision_right()
{
    //lock_guard<mutex> lock(mtx);
    while (true)
    {
        if (!((char)mvinch(yLoc, xLoc + 1) >= (char)97) && !((char)mvinch(yLoc, xLoc) <= (char)122))
        {
            break;
            return;
        }
        
    }
}

void Bolide::detect_collision_left()
{
    //lock_guard<mutex> lock(mtx);
    while (true)
    {
        if (!((char)mvinch(yLoc, xLoc - 1) >= (char)97) && !((char)mvinch(yLoc, xLoc) <= (char)122))
        {
            break;
            return;
        }
    }
}

void Bolide::mvup()
{
    this_thread::sleep_for(chrono::milliseconds(speed*2));
    mut.lock();
    remove_trace();
    yLoc--;
    display();
}

void Bolide::mvdown()
{
    this_thread::sleep_for(chrono::milliseconds(speed*2));
    mut.lock();

    //lock_guard<mutex> lock(mtx);
    //detect_collision_down();

    remove_trace();
    yLoc++;
    display();
}

void Bolide::mvleft()
{
    mut.lock();

    //lock_guard<mutex> lock(mtx);
    //detect_collision_left();

    remove_trace();
    xLoc--;
    display();
}

void Bolide::mvright()
{
    mut.lock();

    //lock_guard<mutex> lock(mtx);
    //detect_collision_right();
    
    remove_trace();
    xLoc++; 
    display();
}

void Bolide::clear()
{
    mut.lock();
    mvprintw(yLoc, xLoc, " ");
    refresh();
    mut.unlock();
}

void Bolide::display()
{
    // pobranie znaku z bieżącej pozycji
    char lastChar = (char)mvinch(yLoc, xLoc);

    attron(COLOR_PAIR(color));
    // nie nadpisujemy konturów trasy
    if (lastChar != (char)124 && lastChar != (char)39)
    {
        mvprintw(yLoc, xLoc, "%c", sign);
    }
    attroff(COLOR_PAIR(color));
    
    refresh();
    mut.unlock();
    this_thread::sleep_for(chrono::milliseconds(speed));
}

void Bolide::movement_long()
{
    // trasa
    for (size_t i = 0; i < 5; i++)
    {
        mvright();
    }

    while (running_loop)  // nieskończona liczba okrążeń - globalna zmienna atomic
    {
        for (size_t i = 0; i < 136; i++)
        {
            mvright();
        }
        for (size_t i = 0; i < 21; i++)
        {
            mvdown();
        }
        for (size_t i = 0; i < 136; i++)
        {
            mvleft();
        }
        for (size_t i = 0; i < 21; i++)
        {
            mvup();
        }
    }

    // usunięcie znaku
    clear();
}

void Bolide::movement_short()
{
    // trasa
    for (size_t i = 0; i < 35; i++)
    {        
        mvdown();
        //detect_collision();
    }
    
    // usunięcie znaku
    clear();
}

int Bolide::getX()
{
    return xLoc;
}

int Bolide::getY()
{
    return yLoc;
}