#ifndef STDLIB_H
#define STDLIB_H

#define RAND_MAX 32767

void * malloc(unsigned int size);
int itoa(int value, char * buffer, int base);
int isAlpha(char c);
int isDigit(char c);
int rand();

#endif
