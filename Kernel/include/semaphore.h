#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <lib.h>
#include <MMU.h>

#define SEM_OPERATIONS 4
#define OPEN 0
#define CLOSE 1
#define POST 2
#define WAIT 3

typedef struct sem_tCDT{
	int value;
	char* name;
	int id;
};

typedef struct sem_tCDT* sem_t;

typedef int (*semOperation)(char*, int);

int semOpen(char* name, int value);
int semClose(char*, int id);
int semPost(char*, int id);
int semWait(char*, int id);
void semOperationsSetup();

extern int testAndSet(uint64_t lock);

#endif