#include <process.h>

int execv(char * filename, int argc, char * argv[]){
	return int80(7, (uint64_t)filename, (uint64_t)argc, (uint64_t)argv);
}

int pthread_create(void *(*startRoutine)(void*), void * arg){
	return int80(10, startRoutine, arg, 0);
}

void kill(int pid){
	int80(11,pid,0,0);
}

char* ps(){
	char* buffer;
	int80(12, buffer,0,0);
	return buffer;
}

void block(int pid){
	int80(13,pid,0,0);
}

void unblock(int pid){
	int80(14,pid,0,0);
}

void yield(){
	int80(15,0,0,0);
}