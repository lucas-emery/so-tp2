#ifndef MMU_H
#define MMU_H

typedef struct {
	int index;
	uint64_t address;
} page_t;

typedef struct {
	page_t * pages;
	int pageCount;
	uint64_t stackPtr;
  uint64_t instPtr;
	uint64_t heapSize;
	uint64_t heapCapacity;
} context_t;

void changePDEPresent(int entry, int present);
void setKernelPresent(int present);
void copyAndExecuteModule(int moduleIndex, int argc, char *argv[]);
void copyAndExecuteDefaultModule();
void changePDE(int entry, uint64_t physAddr, int present);
void changePDEPresent(int entry, int present);
void * initializeKernelBinary();
void * malloc(uint64_t size);
void * getFreePage();
context_t * createFirstThreadContext(int moduleIndex, int argc, char *argv[]);
context_t * createThreadContext(context_t * siblingContext, void * start_routine, void * arg);
void saveContext(uint64_t rsp);
uint64_t loadContext();
void setContext(context_t * newContext);

#endif
