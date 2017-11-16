#include <pthread.h>  
#include <stdio.h>  
  
#define N 5  
#define LEFT (i+N-1)%N  
#define RIGHT (i+1)%N  
#define THINK_TIME 3  
#define EAT_TIME 2  
  
enum { THINKING, HUNGRY, EATING } state[N];  
  
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER, s[N];  
  
void test(int i)  
{  
    if (state[i] == HUNGRY  
     && state[LEFT] != EATING  
     && state[RIGHT] != EATING)  
    {  
        state[i] = EATING;  
        pthread_mutex_unlock(&s[i]);  
    }  
}  
  
void take_forks(int i)  
{  
    pthread_mutex_lock(&mutex);  
    state[i] = HUNGRY;  
    test(i);  
    pthread_mutex_unlock(&mutex);  
    pthread_mutex_lock(&s[i]);  
}  
  
void put_forks(int i)  
{  
    pthread_mutex_lock(&mutex);  
    state[i] = THINKING;  
    test(LEFT);  
    test(RIGHT);  
    pthread_mutex_unlock(&mutex);  
}  
  
void think(int i)  
{  
    printf("philosopher %d is thinking...\n", i);  
    sleep(THINK_TIME);  
}  
  
void eat(int i)  
{  
    printf("philosopher %d is eating...\n", i);  
    sleep(EAT_TIME);  
}  
  
void* phi(void* vargp)  
{  
    int i = *(int*)vargp;  
    while (1)  
    {  
        think(i);  
        take_forks(i);  
        eat(i);  
        put_forks(i);  
    }  
    return NULL;  
}  
  
int main()  
{  
    int i;  
    pthread_t tid[N];  
    for (i = 0; i < N; i++)  
        pthread_create(&tid[i], NULL, phi, (void*)(&i));  
    for (i = 0; i < N; i++)  
        pthread_join(tid[i], NULL);  
    return 0;  
} 