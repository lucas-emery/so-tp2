#ifndef MMU_H
#define MMU_H

void changePDEPresent(int entry, int present);
void setKernelPresent(int present);
void copyAndExecuteModule(int moduleIndex, int argc, char *argv[]);
void copyAndExecuteDefaultModule();
void changePDE(int entry, uint64_t physAddr, int present);
void changePDEPresent(int entry, int present);
void * initializeKernelBinary();
void * malloc(uint64_t size);
void * getFreePage();

typedef struct {
	int index;
	uint64_t address;
} page_t;

typedef struct {
	page_t * pages;
	int pageCount;
	uint64_t stackPtr;
	uint64_t heapSize;
	uint64_t heapCapacity;
} context_t;

#endif
