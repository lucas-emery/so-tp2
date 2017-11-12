#include <semaphore.h>

int sem_open(char* name, int value){
	return int80(16, name, value, 0);
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
