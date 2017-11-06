#include <IPC.h>

static sem_t* semaphores;


int exists(char* name){
	for (int i = 0; i < semCount; ++i){
		if(semaphores[i]->name == name)
			return TRUE;
	}
	return FALSE;
}

int semOpen(char* name, int value){
	if(exists(name))
		return NULL;
	sem_t newSem;
	newSem->name = name;
	newSem->value = value;
	newSem->id = id;
	id++;
	semCount++;
	semaphores = realloc(semaphores, semCount * sizeof(sem_t));
	semaphores[semCount - 1] = newSem;
	//initSemaphore(newSem->id);
	return newSem->id;
}

void semClose(int id){
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

void semPost(int id){
	for (int i = 0; i < semCount; ++i){
		if(semaphores[i]->id == id)
			semaphores[i]->value++;
	}
}

void semWait(int id){
	for (int i = 0; i < semCount; ++i){
		if(semaphores[i]->id == id){
			if(semaphores[i]->value <= 0){
				//semBlock(semaphores[i]->id);
				semaphores[i]->value = -1;
			}
			else
				semaphores[i]->value--;
		}
	}
}

