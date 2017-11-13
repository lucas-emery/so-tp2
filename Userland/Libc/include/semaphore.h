#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdlib.h>

int sem_open(char* name);
int sem_set(int semId, int value);
void sem_close(int id);
void sem_post(int id);
void sem_wait(int id);

#endif
