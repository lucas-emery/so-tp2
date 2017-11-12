//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <strings.h>

#define PHILOSOPHER_COUNT 2

typedef enum {
	Hungry = 0,
	Thinking = 1,
	Eating = 2
} State;

int left(int i);
int right(int i);
void * philosopher(void * id);
void takeForks(int id);
void putForks(int id);
void test(int i);
int randRange(int min, int max);

State state[PHILOSOPHER_COUNT];

int mutex;
int semaphores[PHILOSOPHER_COUNT];
//int philosopherThread[PHILOSOPHER_COUNT];
int philosopherId[PHILOSOPHER_COUNT];

//GUI
void render();
void setPhiloState(int philo, State state);
void setForkState(int fork, int owner);

char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };
State philoState[PHILOSOPHER_COUNT];
int forkState[PHILOSOPHER_COUNT];

void * philosopher(void * id) {
	while(1) {
		//Think
		//sleep(10);	
		//sleep(randRange(5, 10));

		takeForks(*(int*)id);

		//Eat
		//sleep(10);
		//sleep(randRange(5, 10));

		putForks(*(int*)id);
	}
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
	mutex = sem_open("philosophers_mutex", 0);
	char* semaphoreName = "philosopher";

	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		char* number = malloc(10);
		itoa(i,number,10);
		semaphoreName = strcat(semaphoreName, number);
		sem_open(semaphoreName, -1);//Philosophers start not having ownership of the forks		
		semaphoreName = "philosopher";
		free(number);
	}
	
	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		philosopherId[i] = i;
		state[i] = Thinking;
		//pthread_create(philosopherThread[i], philosopher, philosopherId[i]);
	}

	printf("running\n");
	getchar();

	//pthread_exit(NULL);
}

int left(int i) {
	return (i + PHILOSOPHER_COUNT - 1) % PHILOSOPHER_COUNT;
}

int right(int i) {
	return (i + 1) % PHILOSOPHER_COUNT;
}

int randRange(int min, int max) {
	return rand() % (max - min) + min;
}

//GUI
void render() {
	for(int i = 0; i < PHILOSOPHER_COUNT; i++) {
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
