#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STEP 10
#define BUFFERSIZE 1024

void freeParams(char ** argv);
void parseParams(char * command, int * argc, char *** argv);
int exec(char * command);
int execv(char * filename, int argc, char * argv[]);
int pthread_create(void *(*startRoutine)(void*), void * arg);
void kill(int pid);
void block(int pid);
void unblock(int pid);
void exit(int value);
void yield();

#endif
