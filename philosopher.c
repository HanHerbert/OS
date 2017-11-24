
//created by Herbert on 24 Nov,2017

//只有当哲学接的左右两只筷子均处于可用状态时，才允许他拿起筷子。
//这样就可以避免他们同时拿起筷子就餐，导致死锁。

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define phi_num 5
#define think_time 2
#define eat_time 1
#define left (phi_id+phi_num-1)%phi_num
#define right (phi_id+1)%phi_num

enum PHI{ think , hungry , eat };
enum PHI phi_state[phi_num];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t state[phi_num]={PTHREAD_MUTEX_INITIALIZER};

void Do_think(int phi_id){
	printf(" philosopher %d is thinking now !\n",phi_id);
	sleep(think_time);
}
void Do_eat(int phi_id){
	printf("philosopher %d is eating now !\n",phi_id);
	sleep(eat_time);
}
void check_phi_state(int phi_id){
	if(phi_state[phi_id]==hungry&&phi_state[left]!=eat&&phi_state[right]!=eat){
		phi_state[phi_id]=eat;
		pthread_mutex_unlock(&state[phi_id]);
	}
}
void Do_take_forks(int phi_id){
	pthread_mutex_lock(&mutex);
	phi_state[phi_id]=hungry;
	check_phi_state(phi_id);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_lock(&state[phi_id]);
}
void Do_put_forks(int phi_id){
	pthread_mutex_lock(&mutex);
	phi_state[phi_id]=think;
	check_phi_state(left);
	check_phi_state(right);
	pthread_mutex_unlock(&mutex);
}
void *philosopher(void *arg){
	int phi_id=*(int *)arg;
	while(1){
		Do_think(phi_id);
		Do_take_forks(phi_id);
		Do_eat(phi_id);
		Do_put_forks(phi_id);
	}
	return NULL;
}
int main(int argc, char *argv[]){
	int num;
	pthread_t *phi=(pthread_t*)malloc(sizeof(pthread_t)*phi_num);
	int *id=(int *)malloc(sizeof(int)*phi_num);
	for(num=0;num<phi_num;num++){
		id[num]=num;
		pthread_create(&phi[num],NULL,philosopher,(void*)(&id[num]));
	}
	for(num=0;num<phi_num;num++){
		pthread_join(phi[num],NULL);
	}
	return 0;
}