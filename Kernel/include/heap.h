#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif

void initHeap();
void * malloc(uint64_t request);
void * calloc(uint64_t request);
void * realloc(void * ptr, uint64_t size);
void free(void * ptr);

#endif
