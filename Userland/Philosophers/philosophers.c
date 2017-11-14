//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <strings.h>
#include <process.h>

#define MAX 50

typedef struct arguments{
	int id;
	char finishThread;
}arguments;

typedef struct arguments* argumentsPointer;

argumentsPointer philosophers[MAX];

typedef enum {
	Hungry = 0,
	Thinking = 1,
	Eating = 2
} State;

int left(int i);
int right(int i);
void philosopher(argumentsPointer arg);
void takeForks(int id);
void putForks(int id);
void test(int i);
int randRange(int min, int max);
void createPhilosopher();
void deletePhilosopher();

static int philosopherCount;

State state[MAX];

static int mutex;
static int id;
static int semaphores[MAX];
static int philosopherId[MAX];

//GUI
void render();
void setPhiloState(int philo, State state);
void setForkState(int fork, int owner);

char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };
State philoState[MAX];
int forkState[MAX];

static char* createSemaphoreName(int id){
	char* semaphoreName = malloc(30);
	char letra[2] = {'A', 0};
	(letra[0])+= id;
	strcat(semaphoreName,"philosopher");
	strcat(semaphoreName, letra);
	return semaphoreName;
}

void createPhilosopher(){
	while(1){
		keyBlock('p');		
		semaphores[philosopherCount] = sem_open(createSemaphoreName(id));
		philosopherId[philosopherCount] = id;
		state[philosopherCount] = Thinking;
		argumentsPointer arg = malloc(sizeof(arguments));
		arg->id = id;
		arg->finishThread = FALSE;
		philosophers[philosopherCount] = arg;
		id++;
		philosopherCount++;
		pthread_create((function)philosopher, arg);
	}
	pthread_exit();
}

void deletePhilosopher(){
	while(1){
		keyBlock('r');
		if(philosopherCount > 1){
			(philosophers[philosopherCount-1])->finishThread = TRUE;
			sem_close(semaphores[philosopherCount-1]);
			philosopherCount--;
			id--;
		}
	}
	pthread_exit();
}

void philosopher(argumentsPointer arg) {
	int timeout = 100000000;
	while(!arg->finishThread) {
		
		//Think
		if(timeout == 50000000){
      		takeForks(arg->id);
    	}

		//Eat
    	if(timeout == 0){
      		putForks(arg->id);
      		timeout = 100000000;
    	}
    	timeout--;
	}
	pthread_exit();
}

void takeForks(int id) {
	sem_wait(mutex);				//Crit zone
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
	philosopherCount = 0;
	id = 0;
	mutex = sem_open("philosophersMutex");
	sem_set(mutex, 1);
	printf("Press 'p' to add a philosopher and 'r' to remove one\n");
	pthread_create((function)createPhilosopher, (void*)0);
	pthread_create((function)deletePhilosopher, (void*)0);
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
		printf("Philosopher %d: %s \n", i, stateStrings[philoState[i]]);
		printf("Fork - ");

		if (forkState[i] == -1)
			printf("Free\n");
		else
			printf("Owner %d\n", forkState[i]);
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
