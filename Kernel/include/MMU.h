#ifndef MMU_H
#define MMU_H

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
	uint64_t heapSize;
	void * interruptContext;
} context_t;

void changePDE(int entry, uint64_t physAddr, int present);
void changePDEPresent(int entry, int present);
void * initializeKernelBinary();
void * malloc(uint64_t size);
void * getFreePage();
void free(void * ptr);
void * realloc(void * ptr, uint64_t size);
context_t * createFirstThreadContext(int moduleIndex, int argc, char *argv[]);
context_t * createThreadContext(context_t * siblingContext, void * start_routine, void * arg);
void kernelMode();
void userMode();
void setContext(context_t * newContext);
void initSharedMemory();
void enableMemoryProtection();

#endif
