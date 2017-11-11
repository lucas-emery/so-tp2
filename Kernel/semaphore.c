#include <semaphore.h>

typedef struct sem_tCDT{
	int value;
	char* name;
	int id;
}sem_tCDT;

static sem_t* semaphores;
static int semCount = 0;
static int id = 0;
static semOperation semOperations[SEM_OPERATIONS];
volatile int lock = 0;

static int exists(char* name);

static int exists(char* name){
	for (int i = 0; i < semCount; ++i){
		if(semaphores[i]->name == name)
			return TRUE;
	}
	return FALSE;
}

int semOpen(char* name, int value){
	if(exists(name))
		return NULL;
	sem_t newSem = malloc(sizeof(sem_tCDT));
	newSem->name = name;
	newSem->value = value;
	newSem->id = id;
	id++;
	semCount++;
	semaphores = realloc(semaphores, semCount * sizeof(sem_t));
	semaphores[semCount - 1] = newSem;
	initSem(newSem->id);
	return newSem->id;
}

int semClose(char* name, int id){
	for (int i = 0; i < semCount; ++i){
		if(semaphores[i]->id == id){
			destroySem(id);
			free(semaphores[i]);
			for(int j = i; j < semCount-1; j++)
				semaphores[j] = semaphores[j+1];
			semCount--;
			return SUCCESS;
		}
	}
	return FAIL;
}

int semPost(char* name, int id){
	for (int i = 0; i < semCount; ++i){
		if(semaphores[i]->id == id){
			semaphores[i]->value++;
			return SUCCESS;
		}
	}
	return FAIL;
}

int semWait(char* name, int id){
	for (int i = 0; i < semCount; ++i){
		if(semaphores[i]->id == id){
			if(semaphores[i]->value <= 0){
				//semBlock(semaphores[i]->id);
				semaphores[i]->value = -1;
			}
			else
				semaphores[i]->value--;
			return SUCCESS;
		}
	}
	return FAIL;
}

void execute(int operation, char* name, int id){
	if(operation < 0 || operation > 4)
		return;
	testAndSet(&lock);
	(semOperations[operation])(name, id);
	lock = FALSE;
}


void setupSemaphores(){
	semOperations[OPEN] = &semOpen;
	semOperations[CLOSE] = &semClose;
	semOperations[POST] = &semPost;
	semOperations[WAIT] = &semWait;
}