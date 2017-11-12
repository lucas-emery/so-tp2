#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int exec(char * command);
int execv(char * filename, int argc, char * argv[]);
int pthread_create(void *(*startRoutine)(void*), void * arg);
void kill(int pid);
void block(int pid);
void unblock(int pid);
void exit(int value);
void yield();

#endif
