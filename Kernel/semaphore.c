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
static int lock = 0;

void semString(char * buffer){
	strcat(buffer, "Semaphores: ");
	for(int i = 0 ; i < semCount ; i++){
		strcat(buffer, semaphores[i]->name);
		if(i != semCount -1)
			strcat(buffer, ", ");
	}
}

int setValue(char* arg1, int id, int value){
	for (int i = 0; i < semCount; i++){
		if(semaphores[i]->id == id)
			semaphores[i]->value = value;
			return value;
	}
	return -1;
}

int semOpen(char* name, int arg2, int arg3){
	for (int i = 0; i < semCount; i++){
		if(strcmp(semaphores[i]->name,name) == 0)
			return semaphores[i]->id;
	}
	sem_t newSem = malloc(sizeof(sem_tCDT));
	newSem->name = malloc(strlen(name)+1);
	strcpy(newSem->name,name);
	newSem->value = 0;
	newSem->id = id;
	id++;
	semCount++;
	semaphores = realloc(semaphores, semCount * sizeof(sem_t));
	semaphores[semCount - 1] = newSem;
	initSem(newSem->id);
	return newSem->id;
}

int semClose(char* arg1, int id, int arg3){
	for (int i = 0; i < semCount; i++){
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

int semPost(char* arg1, int id, int arg3){
	for (int i = 0; i < semCount; i++){
		if(semaphores[i]->id == id){
			semaphores[i]->value++;
			unblock(id, SEM);
			return SUCCESS;
		}
	}
	return FAIL;
}

int semWait(char* arg1, int id, int arg3){
	for (int i = 0; i < semCount; i++){
		if(semaphores[i]->id == id){
			semaphores[i]->value--;
			if(semaphores[i]->value < 0){
				block(id, SEM);
			}
			return SUCCESS;
		}
	}
	return FAIL;
}

int executeSemaphore(int operation, char* arg1, int arg2, int arg3){
	if(operation < 0 || operation > SEM_OPERATIONS)
		return -1;
	int ret;
	testAndSet(&lock);
	ret = (semOperations[operation])(arg1, arg2, arg3);
	lock = FALSE;
	return ret;
}


void setupSemaphores(){
	semOperations[OPEN] = &semOpen;
	semOperations[CLOSE] = &semClose;
	semOperations[POST] = &semPost;
	semOperations[WAIT] = &semWait;
	semOperations[SET] = &setValue;
}
