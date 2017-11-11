#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stdio.h>

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx); 

int execv(char * filename, int argc, char * argv[]);
int pthread_create(void *(*startRoutine)(void*), void * arg);
void kill(int pid);
void block(int pid);
void unblock(int pid);
void yield();

#endif