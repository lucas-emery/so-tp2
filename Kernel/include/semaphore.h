#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <lib.h>
#include <MMU.h>
#include <scheduler.h>

#define SEM_OPERATIONS 4
#define OPEN 0
#define CLOSE 1
#define POST 2
#define WAIT 3

typedef struct sem_tCDT* sem_t;

typedef int (*semOperation)(char*, int);

extern int testAndSet(uint64_t lock);

int semOpen(char* name, int value);
int semClose(char*, int id);
int semPost(char*, int id);
int semWait(char*, int id);
void setupSemaphores();

#endif