#include <semaphore.h>

typedef struct semCDT{
	int value;
	char* name;
	int id;
} semCDT;

static semADT* semaphores;
static int semCount = 0;
static int id = 0;
static semOperation semOperations[SEM_OPERATIONS];
static int lock = 0;

void semString(char * buffer){
	strcat(buffer, "Semaphores: ");
	char aux[10];
	for(int i = 0 ; i < semCount ; i++){
		strcat(buffer, semaphores[i]->name);
		strcat(buffer, "(");
		uintToBase(semaphores[i]->value,aux,10);
		strcat(aux,")");
		strcat(buffer, aux);
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
	semADT newSem = malloc(sizeof(semCDT));
	newSem->name = malloc(strlen(name)+1);
	strcpy(newSem->name,name);
	newSem->value = 0;
	newSem->id = id;
	id++;
	semCount++;
	semaphores = realloc(semaphores, semCount * sizeof(semADT));
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
