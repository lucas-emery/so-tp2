#ifndef SEMAPHORE_H
#define SEMAPHORE_H

int sem_open(char* name, int value);
void sem_close(int id);
void sem_post(int id);
void sem_wait(int id);

#endif