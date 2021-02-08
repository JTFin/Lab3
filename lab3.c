#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define N 5
#define LEFT (i+N-1) % N
#define RIGHT (i+1) %N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];
int philosopherCount[N] = {0, 1, 2, 3, 4};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s[N];
void take_forks(long i);
void put_forks(long i);
void test(long i);
void *philosopher(void *i);

int main(int argc, char **argv) {

	int i;
	pthread_t thread[N];
	for(i=0;i<N;i++){
        state[i] = THINKING;
	}

	for (i=0;i<N;i++){
		pthread_mutex_init(&s[i], NULL);
	}
	
	for(i=0;i<N;i++){
		pthread_create(&thread[i],NULL,philosopher, (void*) philosopherCount[i]);
	}

	for(i=0;i<N;i++){
       pthread_join(thread[i],NULL);
	}

	pthread_exit(NULL);
}
void *philosopher(void *j) {
	//printf("Philosopher statement\n");
	while(1) {
		long i = (long) j;
		sleep(1); //think
		take_forks(i);
		
		sleep(1); //eat
		put_forks(i);
		
	}
}
void take_forks(long i) {
	//printf("Take forks statement\n");
	pthread_mutex_unlock(&mutex); //down
	state[i] = HUNGRY;
	printf("Philosopher %ld is hungry\n",i);
	test(i);
	pthread_mutex_lock(&mutex); //up
	pthread_mutex_unlock(&s[i]); //down

}
void put_forks(long i) {
	//printf("Put forks statement\n");
	pthread_mutex_unlock(&mutex); //down
	state[i] = THINKING;
	printf("Philosopher %ld is now done eating\n",i+1);
	test(LEFT);		// see if the left neighbour can now eat
	test(RIGHT);	// see if the right neighbour can now eat
	pthread_mutex_lock(&mutex); //up
}
void test(long i) {
	//printf("Test statement\n");
	if(state[i]=HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
		state[i] = EATING;
		printf("Philosopher %ld is now eating\n",i);
		pthread_mutex_lock(&s[i]); //up
		sleep(2); 
	}
}
