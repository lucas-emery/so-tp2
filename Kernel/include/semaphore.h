#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <lib.h>
#include <MMU.h>
#include <scheduler.h>

#define SEM_OPERATIONS 5

typedef struct sem_tCDT* sem_t;

typedef int (*semOperation)(char*, int, int);

extern int testAndSet(uint64_t lock);

void semString(char* buffer);
int setValue(char* arg1, int id, int value);
int semOpen(char* name, int arg2, int arg3);
int semClose(char* arg1, int id, int arg3);
int semPost(char* arg1, int id, int arg3);
int semWait(char* arg1, int id, int arg3);
int executeSemaphore(int operation, char* arg1, int arg2, int arg3);
void setupSemaphores();

#endif
