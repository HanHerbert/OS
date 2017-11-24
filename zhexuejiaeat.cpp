
/*
    created by Herbert on 16 Nov,2017   
    
    预备知识：
        信号量：信号量是管理相应临界区的公有资源，它代表可用资源实体。
            在操作系统中，信号量sem是一个整数。在sem大于等于零时代表
            可供并发进程使用的资源实体的数目，但sem小于零时则代表则代表
            等待使用临界区的进程的数目。

            sem的初始值应该大于零，在建立一个信号量的时候，必须说明信号量
            的代表的意义，以及建立相应的数据结构，以便指向那些等待使用该
            临界区的进程。
        
        P、V原语：
            信号量的数值仅能由P、V的操作改变（P代表 pass，V代表 increment）

            一次P原语操作使得信号量sem减一，一次V原语操作使得信号量sem加一


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
        一人拿一支筷子时所有人都吃不上饭。



*/

/*
让奇数号的哲学家先取右手边的筷子，让偶数号的哲学家先取左手边的筷子。这样，任何一个哲学家
拿到一支筷子后，就已经阻止了他邻座的一个哲学家吃饭的企图，除非某个哲学家一直吃下去，否则不会有人
饿死。
While(true){  
    if( i  %  2 == 0){          //偶数号哲学家  
            P(左边筷子对应的信号量)  
            P(右边筷子对应的信号量)  
            拿起两只筷子吃饭  
            V(右边筷子对应的信号量)  
            V(左边筷子对应的信号量)  
    }else{                  //奇数号哲学家  
            P(右边筷子对应的信号量)  
            P(左边筷子对应的信号量)  
            拿起两只筷子吃饭  
            V(左边筷子对应的信号量)  
            V(右边筷子对应的信号量)  
    }  
    吃饱后思考 
}  
*/

/*
int sem_init(sem_t *sem,int pshared,unsigned int value);//
        sem_init用于对指定信号初始化，pshared为0，
        表示信号在当前进程的多个线程之间共享，value表示初始化信号的值。 

int sem_wait(sem_t *sem);

        sem_wait可以用来阻塞当前线程，直到信号量的值大于0，解除阻塞。
        解除阻塞后，sem的值-1，表示公共资源被执行减少了。
        例如：如果你对一个值为2的信号量调用sem_wait(),线程将会继续执行，
        信号量的值将-1。当初始化value=0后，使用sem_wai会阻塞这个线程，
        这个线程函数就会等待其它线程函数调用
        
int sem_post(sem_t *sem);

        sem_post用于增加信号量的值+1，当有线程阻塞在这个信号量上时，
        调用这个函数会使其中的一个线程不在阻塞，选择机制由线程的调度策略决定。 
*/

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <cstdio>
#define N 5//哲学家人数
#define LEFT i
#define RIGHT (i+1)%N
using namespace std;

class Semaphore {  //定义Semaphore类
private:  
    sem_t sem;  //定义信号量
public:  
    Semaphore(int value = 1) {  
        sem_init(&sem,0,value); //初始化信号量,0表示信号量在线程间共享  
    }  
    void P() {  
        sem_wait(&sem); //等待信号，获取拥有权  
    }  
    void V() {  
        sem_post(&sem); //释放信号，释放拥有权  
    }  
};  

Semaphore mutex[N];//定义N个信号量
pthread_t thread[N];//定义N个线程
int id[N];

void *solve(void* param);//解决算法

void thread_create();//进程创建

void thread_wait();//进程等待

int main(){
    for(int i = 0; i < N; i++) {  
        id[i] = i;  //为哲学家编号
    }  
    thread_create();  
    thread_wait(); 
    return 0;
}

void *solve(void* param){
    int i = *((int *)param);
    while(true){
        if(i % 2 == 0){//如果是偶数，则先取左手边的筷子
            mutex[LEFT].P();
            mutex[RIGHT].P();
            cout<<"哲学家"<<i<<"吃饭"<<endl;
            mutex[RIGHT].V();
            mutex[LEFT].V();
        }
        else{//如果是奇数，则先取右手边的筷子
            mutex[RIGHT].P();  
            mutex[LEFT].P();  
            cout<<"哲学家"<<i<<"吃饭"<<endl; 
            mutex[LEFT].V();  
            mutex[RIGHT].V();  
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

void thread_create() { //进程创建  
    int tmp;  
    for(int i = 0; i < N; i++) {  
        tmp = pthread_create(&thread[i],NULL,solve,&id[i]);  
        if(tmp != 0) {  
            cout<<"线程"<<i<<"创建失败"<<endl; 
        }  
    }  
} 

void thread_wait() {  //进程等待
    for(int i = 0; i < N; i++) {  
        pthread_join(thread[i],NULL);  
        printf("线程%d结束\n",i);  
    }  
}  

