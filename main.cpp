#include "libs.hpp"
#include "Road.hpp"
#include "Bolide.hpp"

bool stop = false;
const int NUM_THREADS = 10;
pthread_mutex_t mutex;


void* movement_func(void* arg)
{
    int count = 0;

    Bolide *bolid = new Bolide(11, 15, 1);

    usleep(300*1000);

    for (size_t i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&mutex);
        bolid->mvright();
        bolid->display();
        refresh();
        pthread_mutex_unlock(&mutex);
        usleep(50*1000);
    }
    while (count++ < 2)
    {
        for (size_t i = 0; i < 64; i++)
        {
            pthread_mutex_lock(&mutex);
            bolid->mvright();
            bolid->display();
            refresh();
            pthread_mutex_unlock(&mutex);
            usleep(50*1000);
        }
        for (size_t i = 0; i < 11; i++)
        {
            pthread_mutex_lock(&mutex);
            bolid->mvdown();
            bolid->display();
            refresh();
            pthread_mutex_unlock(&mutex);
            usleep(150*1000);
        }
        for (size_t i = 0; i < 64; i++)
        {
            pthread_mutex_lock(&mutex);
            bolid->mvleft();
            bolid->display();
            refresh();
            pthread_mutex_unlock(&mutex);
            usleep(50*1000);
        }
        for (size_t i = 0; i < 11; i++)
        {
            pthread_mutex_lock(&mutex);
            bolid->mvup();
            bolid->display();
            refresh();
            pthread_mutex_unlock(&mutex);
            usleep(150*1000);
        }
    }
    
    pthread_exit(0);

    return NULL;
}

int main() 
{
    initscr();
    noecho();
    curs_set(0);
    cbreak();


    Road *road = new Road();
    road->draw_info();
    road->draw_speedway();


    pthread_mutex_init(&mutex, NULL);

    //pthread_t tid0, tid1, tid2;
    //pthread_t * threads[] = {&tid0, &tid1, &tid2};
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        thread_args[i] = i;
        //pthread_create(threads[i], NULL, &movement_func, (void *)&threads[i]);  
        pthread_create(&threads[i], NULL, movement_func, NULL); 
        usleep(1500*1000); 
    }

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        thread_args[i] = i;
        //pthread_join(*threads[i], NULL); 
        pthread_join(threads[i], NULL); 
    }


    if (getchar()) 
    {
        pthread_mutex_destroy(&mutex);
        endwin();

        system("clear");
        system("reset");
        return 0;
    }
}