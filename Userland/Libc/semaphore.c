#include <semaphore.h>

int sem_set(int id, int value){
	return int80(15, id, value, 0);
}

int sem_open(char* name){
	return int80(16, name, 0, 0);
}

void sem_close(int id){
	int80(17, id, 0, 0);
}

void sem_post(int id){
	int80(18, id, 0, 0);
}

void sem_wait(int id){
	int80(19, id, 0, 0);
}
