// #include <pthread.h>  
// #include <stdio.h>  
  
// #define N 5  
// #define LEFT (i+N-1)%N  
// #define RIGHT (i+1)%N  
// #define THINK_TIME 3  
// #define EAT_TIME 2  
  
// enum { THINKING, HUNGRY, EATING } state[N];  
  
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER, s[N];  
  
// void test(int i)  
// {  
//     if (state[i] == HUNGRY  
//      && state[LEFT] != EATING  
//      && state[RIGHT] != EATING)  
//     {  
//         state[i] = EATING;  
//         pthread_mutex_unlock(&s[i]);  
//     }  
// }  
  
// void take_forks(int i)  
// {  
//     pthread_mutex_lock(&mutex);  
//     state[i] = HUNGRY;  
//     test(i);  
//     pthread_mutex_unlock(&mutex);  
//     pthread_mutex_lock(&s[i]);  
// }  
  
// void put_forks(int i)  
// {  
//     pthread_mutex_lock(&mutex);  
//     state[i] = THINKING;  
//     test(LEFT);  
//     test(RIGHT);  
//     pthread_mutex_unlock(&mutex);  
// }  
  
// void think(int i)  
// {  
//     printf("philosopher %d is thinking...\n", i);  
//     sleep(THINK_TIME);  
// }  
  
// void eat(int i)  
// {  
//     printf("philosopher %d is eating...\n", i);  
//     sleep(EAT_TIME);  
// }  
  
// void* phi(void* vargp)  
// {  
//     int i = *(int*)vargp;  
//     while (1)  
//     {  
//         think(i);  
//         take_forks(i);  
//         eat(i);  
//         put_forks(i);  
//     }  
//     return NULL;  
// }  
  
// int main()  
// {  
//     int i;  
//     pthread_t tid[N];  
//     for (i = 0; i < N; i++)  
//         pthread_create(&tid[i], NULL, phi, (void*)(&i));  
//     for (i = 0; i < N; i++)  
//         pthread_join(tid[i], NULL);  
//     return 0;  
// } 


/*
    created by Herbert on 16 Nov,2017   

        问题描述：设有五个哲学家，共享一张放有五把椅子的桌子，没人分得一把椅子。但是，桌子上
    总共有五支筷子，在每人两边分开各放一支。哲学家们在肚子饥饿时才会试图分两次从两边拾起筷子
    就餐。
        条件如下：
            （1）只有拿到两支筷子时，哲学家才能吃饭。
            （2）如果筷子已经在他人手上，则该哲学家必须等到他人吃完之后才能拿到筷子。
            （3）任一哲学家在自己未拿到两支筷子吃饭之前，绝不放下自己手中的筷子。
        试：
            （1）描述一个保证不会出现两个邻座同时要求吃饭的通信算法
            （2）描述一个既没有两邻座同时吃饭，又没有人饿死（永远拿不到筷子）的算法。
            （3）在什么情况下，五个哲学家全部吃不上饭。
*/

/*
    解答：
    （1）
        设信号量c[0]~c[4]，初始值均为1,分别表示I号筷子被拿（I=0,1,2,3,4）。

        send(I):第I个哲学家要吃饭
        begin:
            P(c[i]);
            P(c[I+1 mod 5]);
            eat;
            V(c[I+1 mod 5]);
            V(c[I]);
        end
        该过程能保证两邻座不同时吃饭，但会出现五个哲学家一人拿一支筷子，谁也吃不上饭的死锁情形。
    （2）
        解决的思路如下：
                让奇数号的哲学家先取右手边的筷子，让偶数号的哲学家先取左手边的筷子。这样，任何一个哲学家
            拿到一支筷子后，就已经阻止了他邻座的一个哲学家吃饭的企图，除非某个哲学家一直吃下去，否则不会有人
            饿死。
        
            send(I):
            begin
                if I mod 2 == 0
                then 
                {
                    P(c[I]);
                    P(c[I+1] mod 5);
                    eat;
                    V(c[I]);
                    V(c[I+1] mod 5);
                }
                else
                {
                    P(c[I+1 mod 5]);
                    P(c[I]);
                    eat;
                    V(c[I+1 mod 5]);
                    V(c[I]);
                }
            end
    （3）
        一人拿一支筷子时吃不上饭。



*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define PHILO_NUM 5
sem_t chopstick[PHILO_NUM];
sem_t mutex;
sem_t room;

void *philosopher(void *arg)
{
    int num;
    num = *(int *)arg;
    while(1)
    {
        sem_wait(&room);//最多只允许4个哲学家拿起左边的筷子
        sem_wait(&mutex);//互斥锁保证原子操作
        sem_wait(&chopstick[num]);
        sem_wait(&chopstick[(num+1)%PHILO_NUM]);
        sem_post(&mutex);
        printf("philosopher %d eat\n",num);
        sem_post(&chopstick[num]);
        sem_post(&chopstick[(num+1)%PHILO_NUM]);        
        sem_post(&room);
        sleep(1);
    }
}

int main()
{
    pthread_t philo_id[PHILO_NUM];
    int i = 0;
    int arg[PHILO_NUM];
    for(i=0;i<PHILO_NUM;i++)
    {
        sem_init(chopstick+i,0,1);
        arg[i] = i;
    }
    sem_init(&mutex,0,1);
    sem_init(&room,0,4);
    for(i=0;i<PHILO_NUM;i++)
    {
        pthread_create(philo_id+i,NULL,philosopher,(void *)&arg[i]);
    }

    while(1)
    {
        sleep(1);
    }

    return 0;
}