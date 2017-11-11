#ifndef THREADS_H
#define THREADS_H

#include <lib.h>
#include <MMU.h>
#include <pcb.h>

#define RUNNING 0
#define BLOCKED 1
#define READY 2
#define NEW 3

typedef struct tcbCDT{
	int tid;
	int pid;
	context_t * context;
	int state;
	int privilage;
} tcbCDT;

typedef struct tcbCDT* tcbADT;


#include <scheduler.h>

tcbADT createFirstTCB(int privilege,int pid, int moduleIndex, int argc, char *argv[]);
tcbADT createTCB(int privilege, void * start_routine, void * arg);

#endif