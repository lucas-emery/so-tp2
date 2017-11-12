#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

#define RAND_MAX 32767

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);
void * malloc(unsigned int size);
int itoa(uint64_t value, char * buffer, int base);
int isAlpha(char c);
int isDigit(char c);
int rand();

#endif
