#ifndef PROCESS_H
#define PROCESS_H

extern void int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx); 

int fork();
void kill(int pid);
void block(int pid);
void unblock(int pid);
void yield();

#endif