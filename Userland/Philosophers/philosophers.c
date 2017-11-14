//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <strings.h>
#include <process.h>

#define MAX 20

typedef enum {
	Hungry = 0,
	Thinking = 1,
	Eating = 2
} State;

int left(int i);
int right(int i);
void philosopher(int id);
void takeForks(int id);
void putForks(int id);
void test(int i);
int randRange(int min, int max);

static int philosopherCount;

State state[MAX];

static int mutex;
static int semaphores[MAX];
static int philosopherId[MAX];

//GUI
void render();
void setPhiloState(int philo, State state);
void setForkState(int fork, int owner);

char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };
State philoState[MAX];
int forkState[MAX];

void philosopher(int id) {
	int timeout = 10000000;
	while(1) {
		
		//Think
		if(timeout == 5000000){
			printf("tik\n");
      		takeForks(*(int*)id);
      		timeout=10000000;
    	}

		//Eat
    	if(timeout == 0){
    		printf("eat\n");
      		putForks(*(int*)id);
    	}
    	timeout--;
	}
	pthread_exit();
}

void takeForks(int id) {
	sem_wait(mutex);				//Crit zone
	printf('take');
	//Set state
	state[id] = Hungry;
	setPhiloState(id, Hungry);
	render();

	test(id);								//Try to acquire forks
	sem_post(mutex);			//Crit zone exit
	sem_wait(semaphores[id]);	//Locks if forks not acquired
}

void putForks(int id) {
	sem_wait(mutex);				//Crit zone
	printf("put");
	//Set state
	state[id] = Thinking;
	//Think and release forks
	setPhiloState(id, Thinking);
	setForkState(left(id), -1);
	setForkState(id, -1);
	render();

	test(left(id));							//Try to acquire forks for left
	test(right(id));						//Try to acquire forks for right
	sem_post(mutex);			//Crit zone exit
}

void test(int id) {
	if (state[id] == Hungry &&				//Philosopher is hungry
		state[left(id)] != Eating &&		//Both philosophers at
		state[right(id)] != Eating) {		//left and right are not eating

		//Set state
		state[id] = Eating;					//Philosopher can eat!
		//Take forks and eat
		setPhiloState(id, Eating);
		setForkState(left(id), id);
		setForkState(id, id);
		render();

		sem_post(semaphores[id]);	//Forks acquired, unlock
	}
}

int main(int argc, char ** argv) {
	//Setup
	philosopherCount = 4;
	mutex = sem_open("philosophersMutex");
	char letra[2] = {'A',0};

	for (int i = 0; i < philosopherCount; i++) {
		char* semaphoreName = malloc(30);
		strcat(semaphoreName,"philosopher");
		strcat(semaphoreName, letra);
		(letra[0])++;
		semaphores[i] = sem_open(semaphoreName);//Philosophers start not having ownership of the forks
	}

	for (int i = 0; i < philosopherCount; i++) {
		philosopherId[i] = i;
		printf("%d\n", philosopherId[i]);
		state[i] = Thinking;
		pthread_create((function)philosopher, i);
	}

	printf("running\n");
	//getchar();
	//pthread_exit();
	while(1);
	return 0;
}

int left(int i) {
	return (i + philosopherCount - 1) % philosopherCount;
}

int right(int i) {
	return (i + 1) % philosopherCount;
}

int randRange(int min, int max) {
	return rand() % (max - min) + min;
}

//GUI
void render() {
	for(int i = 0; i < philosopherCount; i++) {
		printf("Philosopher %d: %s\n", i, stateStrings[philoState[i]]);
		printf("Fork - ");

		if (forkState[i] == -1)
			printf("Free\n");
		else
			printf("Owner %d\n", forkState[forkState[i]]);
	}

	putchar('\n');
	putchar('\n');
}

void setPhiloState(int philo, State state) {
	philoState[philo] = state;
}

void setForkState(int fork, int owner) {
	forkState[fork] = owner;
}
