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
	uint64_t heapBase;
	uint64_t heapSize;
	void * interruptStack;
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
void kernelMode();
void userMode();
void setContext(context_t * newContext);

extern uint64_t cleanReturnAddress();
extern void injectReturnAddress(uint64_t returnAddress);

#endif
