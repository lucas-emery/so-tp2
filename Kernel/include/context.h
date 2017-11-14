#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdint.h>

typedef struct {
	int index;
	uint64_t address;
} page_t;

typedef struct {
	page_t dataPage;
	page_t heapPage;
	page_t stackPage;
	page_t kernelPage;
	uint64_t heapBase;
	void * interruptContext;
} context_t;

context_t * createFirstThreadContext(int moduleIndex, int argc, char *argv[]);
context_t * createThreadContext(context_t * siblingContext, void * start_routine, void * arg);
void freeProcessContext(context_t * context);
void freeThreadContext(context_t * context);
void kernelMode();
void userMode();
void sharedMode();
void setContext(context_t * newContext);
void initKernelContext();
void initSharedMemory();

#endif
