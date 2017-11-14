#ifndef THREADS_H
#define THREADS_H

typedef struct tcbCDT* tcbADT;

#include <lib.h>
#include <MMU.h>
#include <pcb.h>
#include <context.h>

typedef struct tcbCDT{
	int tid;
	int pid;
	context_t * context;
	int state;
	int privilage;
} tcbCDT;


#include <scheduler.h>

tcbADT createFirstTCB(int privilege,int pid, int moduleIndex, int argc, char *argv[]);
tcbADT createTCB(int privilege, void * start_routine, void * arg);
int createThread(void *(*startRoutine)(void*), void * arg);

#endif
