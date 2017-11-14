#ifndef MMU_H
#define MMU_H

#define PAGESIZE 0x200000
#define KERNEL 0x0
#define KERNEL_HEAP 0x200000
#define SYSCALL_STACK 0x400000
#define	KERNEL_STACK 0x600000
#define SHARED_MEMORY 0x800000
#define EXEC_MEM_ADDR 0xA00000
#define ROM 0xC00000
#define MAPPEDMEMORY 0x100000000
#define USER_HEAP (MAPPEDMEMORY/2)
#define STACKBASE (MAPPEDMEMORY - sizeof(uint64_t))
#define SYSCALL_STACKBASE (SYSCALL_STACK + PAGESIZE - sizeof(uint64_t))
#define	KERNEL_STACKBASE (KERNEL_STACK + PAGESIZE - sizeof(uint64_t))
#define SYSCALL_STACK_BOTTOM (SYSCALL_STACK + PAGESIZE)
#define KERNEL_STACK_BOTTOM (KERNEL_STACK + PAGESIZE)
#define AVOID_BSS 1
#define PRESENT 1
#define NOT_PRESENT 0

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

void changePDE(int entry, uint64_t physAddr, int present);
void changePDEPresent(int entry, int present);
void * initializeKernelBinary();
void * malloc(uint64_t size);
void free(void * ptr);
void * realloc(void * ptr, uint64_t size);
context_t * createFirstThreadContext(int moduleIndex, int argc, char *argv[]);
context_t * createThreadContext(context_t * siblingContext, void * start_routine, void * arg);
void freeProcessContext(context_t * context);
void freeThreadContext(context_t * context);
void kernelMode();
void userMode();
void setContext(context_t * newContext);
void initSharedMemory();
void enableMemoryProtection();
void setupTSS();
void setupGDT();

#endif
