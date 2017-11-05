#include <process.h>

int fork(){
	int pid;
	int80(10,&pid,0,0);
	return pid;
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