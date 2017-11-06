#include <IPC.h>
#include <lib.h>

typedef struct sem_t{
	int value;
	char* name;
	int id;
};

int exists(){
	for (int i = 0; i < semCount; ++i){
		if(semaphores[i]->name == name)
			return TRUE;
	}
	return FALSE;
}

int sem_open(char* name, int value){
	if(exists(name))
		return NULL;
	sem_t newSem;
	newSem->name = name;
	newSem->value = value;
	newSem->id = id;
	id++;
	semCount++;
	semaphores = realloc(semaphores, semCount * sizeof(*sem_t));
	semaphores[semCount - 1] = newSem;
	//createQueue(newSem->id);
	return newSem->id;
}

void closeMutex(int id){
	int found = FALSE;
	for (int i = 0; i < semCount || !found; ++i){
		if(semaphores[i]->id == id){
			free(semaphores[i]);
			for(int j = i; j < semCount-1; j++)
				semaphores[j] = semaphores[j+1];
			semCount--;
			found = TRUE;
		}
	}
}

void up(int id){
	for (int i = 0; i < semCount; ++i){
		if(semaphores[i]->id == id)
			semaphores[i]->value++;
	}
}

void down(int id){
	for (int i = 0; i < semCount; ++i){
		if(semaphores[i]->id == id){
			if(semaphores[i]->value <= 0){
				//sem_block(semaphores[i]->id);
				semaphores[i]->value = -1;
			}
			else
				semaphores[i]->value--;
		}
	}
}

