#ifndef SYSCALLSIPC_H
#define SYSCALLSIPC_H

typedef struct sem_t;

static int semCount = 0;
static int id = 0;

sem_t* semaphores;
int sem_open(char* name, int value);
int sem_close(int id);
int sem_post(int id);
int sem_wait(int id);

#endif