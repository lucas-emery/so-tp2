#include <heap.h>
#include <context.h>
#include <MMU.h>
#include <lib.h>

extern context_t * context;

void initHeap() {
	uint64_t * heapSize = (uint64_t*)context->heapBase;
	*heapSize = sizeof(uint64_t);
}

void * malloc(uint64_t request) {
	uint64_t * heapSize = (uint64_t*)context->heapBase;
	uint64_t futureSize = *heapSize + request;
	if (futureSize > PAGESIZE) {
		return NULL;
	}

	uint64_t blockAddress = context->heapBase + *heapSize;
	*heapSize = futureSize;

	return blockAddress;
}

void * calloc(uint64_t request){
	void * blockAddress = malloc(request);

	uint8_t * aux = (uint8_t *) blockAddress;
	for(int i = 0; i < request ; i++){
		aux[i] = 0;
	}
	return blockAddress;
}

void * realloc(void * ptr, uint64_t size) {
	void *  newptr = malloc(size);
	memcpy(newptr, ptr, size);
	return newptr;
}

void free(void * ptr) {

}
