#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STEP 10
#define BUFFERSIZE 1024

typedef void * (*function)(void *);

void freeParams(char ** argv);
void parseParams(char * command, int * argc, char *** argv);
int exec(char * command);
int execv(char * filename, int argc, char * argv[]);
int pthread_create(void *(*startRoutine)(void*), void * arg);
void pthread_exit();
int getPid();
int getVar(char * name,char* buffer);
void ps(char * buffer);
void kill(int pid);
void block(int pid);
void unblock(int pid);
void exit(int value);

#endif
