#include "Bolide.hpp"

// generowanie nowych pojazdów
extern bool running_loop;

// liczniki
extern unsigned int vertical_first_count;
extern unsigned int vertical_second_count;
extern unsigned int horizontal_first_count;
extern unsigned int horizontal_second_count;

// mapy bolidów
extern map <int, tuple<int, int, char, int>> bolide1_map;
extern map <int, tuple<int, int, char, int>> bolide2_map;

// mutexy chronią współdzielone dane 
// przed równoczesnym dostępem wielu wątków
extern mutex h1_m, h2_m, v1_m, v2_m, h1_mq, h2_mq, v1_mq, v2_mq;;

// zmienne warynkowe to obiekty które mogą blokować wątek 
// wywołujący dopóki nie zostanie powiadomiony o wznowieniu
extern condition_variable h1_cv, h2_cv, v1_cv, v2_cv, h1_cvq, h2_cvq, v1_cvq, v2_cvq;

// kolejki pojazdów
extern queue<int> h1_q;
extern queue<int> h2_q;
extern queue<int> v1_q;
extern queue<int> v2_q;


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


// SEKCJA KRYTYCZNA - czekanie w kolejce
        // zablokowanie wątku
        unique_lock<mutex> ul1q(h1_mq);

        // dodanie samochodu do kolejki
        h1_q.push(id);

        if (h1_q.front() != id)
        {
            // czekamy aż ten samochód będzie pierwszy w kolejce
            h1_cvq.wait(ul1q, [&] {return h1_q.front() == id;});
        }

        ul1q.unlock();
// KONIEC SEKCJI KRYTYCZNEJ - czekanie w kolejce


// SEKCJA KRYTYCZNA - czekanie na wolne skrzyżowanie
        // zablokowanie wątku
        unique_lock<mutex> ul1(h1_m);
  
        if (vertical_first_count > 0 || horizontal_first_count > 5)
        {   
            // czekanie na brak pojazdów kolizyjnych oraz limit na naszym pasie ruchu
            h1_cv.wait(ul1, [] {return vertical_first_count == 0 && horizontal_first_count < 6;});
        }

        // wjechanie na skrzyżowanie i usunięcie samochodu z kolejki 
        h1_q.pop();
        // powiadomienie pierwszego samochodu czekającego w kolejce
        h1_cvq.notify_one();

        horizontal_first_count++;
        
        ul1.unlock();
// KONIEC SEKCJI KRYTYCZNEJ - czekanie na wolne skrzyżowanie
 

        // jest na skrzyżowaniu
        for (size_t x = 89; x < 100; x++)
        {
            mvright();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 1);
        }
        // opuszcza skrzyżowanie

        h1_m.lock();
        horizontal_first_count--;
        h1_m.unlock();

        // informujemy pierwszy pojazd w kolejce o przejezdności
        if (horizontal_first_count < 6)
        {
            h1_cv.notify_one();
        }

        // powiadomienie o przejezdności samochodów kolizyjnych
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


// SEKCJA KRYTYCZNA - czekanie w kolejce
        // zablokowanie wątku
        unique_lock<mutex> ul2q(h2_mq);

        // dodanie samochodu do kolejki
        h2_q.push(id);

        if (h2_q.front() != id)
        {
            // czekamy aż ten samochód będzie pierwszy w kolejce
            h2_cvq.wait(ul2q, [&] {return h2_q.front() == id;});
        }

        ul2q.unlock();
// KONIEC SEKCJI KRYTYCZNEJ - czekanie w kolejce


// SEKCJA KRYTYCZNA - czekanie na wolne skrzyżowanie
        // zablokowanie wątku
        unique_lock<mutex> ul2(h2_m);

        if (vertical_second_count > 0 || horizontal_second_count > 5)
        {
            // czekanie na brak pojazdów kolizyjnych oraz limit na naszym pasie ruchu
            h2_cv.wait(ul2, [] {return vertical_second_count == 0 && horizontal_second_count < 6;});
        }

        // wjechanie na skrzyżowanie i usunięcie samochodu z kolejki
        h2_q.pop();
        // powiadomienie pierwszego samochodu czekającego w kolejce
        h2_cvq.notify_one();

        horizontal_second_count++;

        ul2.unlock();
// KONIEC SEKCJI KRYTYCZNEJ - czekanie na wolne skrzyżowanie


        // jest na skrzyżowaniu
        for (size_t x = 35; x < 46; x++)
        {
            mvleft();
            bolide1_map[id] = make_tuple(xLoc, yLoc, sign, 3);
        }
        // opuszcza skrzyżowanie
        
        h2_m.lock();
        horizontal_second_count--;
        h2_m.unlock();

        // informujemy pierwszy pojazd w kolejce o przejezdności
        if (horizontal_second_count < 6)
        {
            h2_cv.notify_one();
        }

        // powiadomienie o przejezdności samochodów kolizyjnych
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


// SEKCJA KRYTYCZNA - czekanie w kolejce
        // zablokowanie wątku
        unique_lock<mutex> ul1q(v1_mq);

        vertical_first_count++;

        // dodanie samochodu do kolejki
        v1_q.push(id);

        if (v1_q.front() != id)
        {
            // czekamy aż ten samochód będzie pierwszy w kolejce
            v1_cvq.wait(ul1q, [&] {return v1_q.front() == id;});
        }

        ul1q.unlock();
// KONIEC SEKCJI KRYTYCZNEJ - czekanie w kolejce


// SEKCJA KRYTYCZNA - czekanie na wolne skrzyżowanie
    // zablokowanie wątku
    unique_lock<mutex> ul1(v1_m);

    if (horizontal_first_count > 0 || vertical_first_count > 5)
    {
        // czekanie na brak pojazdów kolizyjnych oraz limit na naszym pasie ruchu
        v1_cv.wait(ul1, [] {return horizontal_first_count == 0 && vertical_first_count < 6;}); 
    }
    
    // wjechanoe na skrzyżowanie i usunięcie samochodu z kolejki
    v1_q.pop();
    // powiadomienie pierwszego samochodu czekającego w kolejce
    v1_cvq.notify_one();

    ul1.unlock();
// KONIEC SEKCJI KRYTYCZNEJ - czekanie na wolne skrzyżowanie


    // jest na skrzyżownaiu
    for (size_t i = 3; i < 9; i++)
    {
        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }
    // opuszcza skrzyżowanie

    v1_m.lock();
    vertical_first_count--;
    v1_m.unlock();

    // informujemy pierwszy pojazd w kolejce o przejezdności
    if (vertical_first_count < 6)
    {
        v1_cv.notify_one();
    }

    // powiadomienie o przejezdności samochodów kolizyjnych
    if (vertical_first_count == 0)
    {
        h1_cv.notify_one();
    }


    for (size_t i = 8; i < 23; i++)
    {
        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }


// SEKCJA KRYTYCZNA - czekanie w kolejce
        // zablokowanie wątku
        unique_lock<mutex> ul2q(v2_mq);

        vertical_second_count++;

        // dodanie samochodu do kolejki
        v2_q.push(id);

        if (v2_q.front() != id)
        {
            // czekamy aż ten samochód będzie pierwszy w kolejce
            v2_cvq.wait(ul2q, [&] {return v2_q.front() == id;});
        }

        ul2q.unlock();
// KONIEC SEKCJI KRYTYCZNEJ - czekanie w kolejce


// SEKCJA KRYTYCZNA - czekanie na wolne skrzyżowanie
    // zablokowanie wątku
    unique_lock<mutex> ul2(v2_m);

    if (horizontal_second_count > 0 || vertical_second_count > 5)
    {
        // czekanie na brak pojazdów kolizyjnych oraz limit na naszym pasie ruchu
        v2_cv.wait(ul2, [] {return horizontal_second_count == 0 && vertical_second_count < 6;});
    }

    // wjechanie na skrzyżowanie i usunięcie samochodu z kolejki
    v2_q.pop();
    // powiadomienie pierwszego samochodu czekającego w kolejce
    v2_cvq.notify_one();

    ul2.unlock();
// KONIEC SEKCJI KRYTYCZNEJ - czekanie na wolne skrzyżowanie

    
    // jest na skrzyżownaiu
    for (size_t i = 22; i < 28; i++)
    {
        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }
    // opuszcza skrzyżowanie

    v2_m.lock();
    vertical_second_count--;
    v2_m.unlock();

    // informujemy pierwszy pojazd w kolejce o przejezdności
    if (vertical_second_count < 6)
    {
        v2_cv.notify_one();
    }

    // powiadomienie o przejezdności samochodów kolizyjnych
    if (vertical_second_count == 0)
    {
        h2_cv.notify_one();
    }


    for (size_t i = 27; i < 31; i++)
    {
        mvdown();
        bolide2_map[id] = make_tuple(xLoc, yLoc, sign, 2);
    }
}