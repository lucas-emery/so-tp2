#include <context.h>
#include <MMU.h>
#include <heap.h>
#include <lib.h>
#include <pageAllocator.h>

context_t * context = AVOID_BSS;
static context_t * kernelContext = AVOID_BSS;
static context_t * processContext = AVOID_BSS;
static context_t * sharedContext = AVOID_BSS;

extern void ** moduleAddresses;

context_t * createContext(uint64_t dataPageAddress, uint64_t heapPageAddress) {
	if(heapPageAddress == NULL) {
		heapPageAddress = getFreePage();
	}

	uint64_t stackPageAddress = getFreePage();
	uint64_t kernelPageAddress = getFreePage();

	context_t * newContext = malloc(sizeof(context_t));
	newContext->dataPage.index = EXEC_MEM_ADDR/PAGESIZE;
	newContext->dataPage.address = dataPageAddress;
	newContext->heapPage.index = USER_HEAP/PAGESIZE;
	newContext->heapPage.address = heapPageAddress;
	newContext->stackPage.index = STACKBASE/PAGESIZE;
	newContext->stackPage.address = stackPageAddress;
	newContext->kernelPage.index = SYSCALL_STACK/PAGESIZE;
	newContext->kernelPage.address = kernelPageAddress;
	newContext->heapBase = USER_HEAP;

	return newContext;
}

void buildThreadStack(uint64_t rdi, uint64_t rsi, uint64_t rip, context_t * threadContext) {
	void * rsp = buildStack(rdi, rsi, rip, STACKBASE);

	threadContext->interruptContext = malloc(CONT_SW_STACK_SIZE);
	memcpy(threadContext->interruptContext, rsp, CONT_SW_STACK_SIZE);
	setStackPtr(rsp - CONT_SW_STACK_SIZE); //Clean stack
}

char** moveArgsToActiveHeap(int argc, char * argv[]) {
	char ** new = argv;
  if(argc > 0) {
    new = malloc(argc * sizeof(char **));
    if(new == 0)
      return argv;

    for(int i = 0; i < argc; i++) {
      size_t len = strlen(argv[i]) + 1;
      new[i] = malloc(len * sizeof(char));
      if(new[i] == 0)
        return argv;
      memcpy(new[i], argv[i], len * sizeof(char));
    }
  }
  return new;
}

void changeHeap(context_t * newContext) {
	context = newContext;
	loadPage(newContext->heapPage);
	flushPaging();
}

void restoreProcessHeap() {
	if(processContext != NULL) {
		loadPage(processContext->heapPage);
		flushPaging();
	}
}

void changeDataPage(uint64_t address) {
	changePDE(EXEC_MEM_ADDR / PAGESIZE, address, PRESENT);
	flushPaging();
}

void restoreProcessData() {
	if(processContext != NULL) {
		loadPage(processContext->dataPage);
		flushPaging();
	}
}

uint64_t copyModule(int moduleIndex) {
	uint64_t dataPageAddress = getFreePage();
	changeDataPage(dataPageAddress);
	memcpy((void *)EXEC_MEM_ADDR, moduleAddresses[moduleIndex], PAGESIZE);
	restoreProcessData();
	return dataPageAddress;
}

context_t * createFirstThreadContext(int moduleIndex, int argc, char *argv[]) {
	uint64_t dataPageAddress = copyModule(moduleIndex);
	context_t * newContext = createContext(dataPageAddress, NULL);

	sharedMode();
	argv = moveArgsToActiveHeap(argc, argv);
	changeHeap(newContext);
	initHeap();
	argv = moveArgsToActiveHeap(argc, argv);
	restoreProcessHeap();
	kernelMode();

	buildThreadStack((uint64_t)argc, (uint64_t)argv, EXEC_MEM_ADDR, newContext);
	return newContext;
}

context_t * createThreadContext(context_t * siblingContext, void * start_routine, void * arg) {
	context_t * newContext = createContext(siblingContext->dataPage.address, siblingContext->heapPage.address);
	buildThreadStack((uint64_t)arg, NULL, (uint64_t)start_routine, newContext);
	return newContext;
}

void freeProcessContext(context_t * context) {
	freePage(context->dataPage.address);
	freePage(context->heapPage.address);
}

void freeThreadContext(context_t * context) {
	freePage(context->stackPage.address);
	freePage(context->kernelPage.address);
}

void kernelMode() {
	context = kernelContext; //Just affects the heap
}

void userMode() {
	context = processContext; //Just affects the heap
}

void sharedMode() {
	context = sharedContext; //Just affects the heap
}

void saveContext() {
	memcpy(processContext->interruptContext, KERNEL_STACK_BOTTOM - CONT_SW_STACK_SIZE, CONT_SW_STACK_SIZE);
}

void loadContext() {
	loadPage(processContext->dataPage);
	loadPage(processContext->heapPage);
	loadPage(processContext->stackPage);
	loadPage(processContext->kernelPage);
	memcpy(KERNEL_STACK_BOTTOM - CONT_SW_STACK_SIZE, processContext->interruptContext, CONT_SW_STACK_SIZE);
}

void setContext(context_t * newContext) {
	if(processContext != NULL)
		saveContext();

	processContext = newContext;

	loadContext();
	flushPaging();
}

void initKernelContext() {
	context_t tempContext; //We need a working heap to create kernelContext
	tempContext.heapBase = KERNEL_HEAP;
	context = &tempContext;

	initHeap();

	kernelContext = (context_t *) malloc(sizeof(context_t));
	kernelContext->heapBase = KERNEL_HEAP;

	context = kernelContext;
	processContext = NULL;
}

void initSharedMemory() {
	sharedContext = (context_t *) malloc(sizeof(context_t));
	sharedContext->heapBase = SHARED_MEMORY;

	changePDE(SHARED_MEMORY/PAGESIZE, getFreePage(), PRESENT);
	flushPaging();

	context_t * oldContext = context;
	context = sharedContext;
	initHeap();
	context = oldContext;
}
