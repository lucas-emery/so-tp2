#ifndef THREADS_H
#define THREADS_H

#include <lib.h>
#include <MMU.h>
#include <scheduler.h>
#include <process.h>

#define RUNNING 0
#define BLOCKED 1
#define READY 2
#define NEW 3

typedef struct tcbCDT;
typedef struct tcbCDT* tcbADT;

tcbADT createFirstTCB(int privilege, int moduleIndex, int argc, char *argv[]);
tcbADT createTCB(int privilege, void * start_routine, void * arg);

#endif