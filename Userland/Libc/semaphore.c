#include <semaphore.h>

int sem_set(int id, int value){
	return int80(15, (uint64_t) id, (uint64_t) value, 0);
}

int sem_open(char* name){
	return int80(16, (uint64_t) name, 0, 0);
}

void sem_close(int id){
	int80(17, (uint64_t) id, 0, 0);
}

void sem_post(int id){
	int80(18, (uint64_t) id, 0, 0);
}

void sem_wait(int id){
	int80(19, (uint64_t) id, 0, 0);
}
