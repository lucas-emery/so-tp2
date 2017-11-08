#include <stdint.h>
#include <moduleLoader.h>
#include <lib.h>
#include <terminal.h>
#include <naiveConsole.h>
#include <terminal.h>
#include <MMU.h>

#define PD_ADDR 0x10000
#define PAGESIZE 0x200000
#define KERNEL_HEAP 0x200000
#define KERNEL_STACK 0x400000
#define EXEC_MEM_ADDR 0x600000
#define ROM 0x800000
#define MAPPEDMEMORY 0x100000000
#define USER_HEAP (MAPPEDMEMORY/2)
#define STACKBASE (MAPPEDMEMORY - sizeof(uint64_t))
#define KERNEL_STACKBASE (KERNEL_STACK + PAGESIZE - sizeof(uint64_t))
#define PRESENT 1
#define NOT_PRESENT 0
#define AVOID_BSS 1
#define INITIAL_PC 3
#define HEAP_PAGE_IDX 0
#define STACK_PAGE_IDX 1
#define DATA_PAGE_IDX 2
#define EMPTY 0
#define NULL 0
#define INT_STACK_SIZE (20 * 8)

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern void hang();
extern uint64_t getStackPtr();
extern void setStackPtr(uint64_t rsp);
extern void * buildStack(uint64_t argc, uint64_t argv, uint64_t rip, uint64_t rsp);
extern uint64_t swapStack(uint64_t newStackPtr, int pageIndex, uint64_t pageAddress);

typedef int (*EntryPoint)(int argc, char *argv[]);

char* moduleNames[] = {"shell", "sampleDataModule", "sampleCodeModule", "hello", "help", "date", "time", "clear", "roflmao",0};
void ** moduleAddresses;
static context_t * context = AVOID_BSS;
static context_t * kernelContext = AVOID_BSS;
static context_t * processContext = AVOID_BSS;

void copyAndExecuteDefaultModule(){
	memcpy(EXEC_MEM_ADDR, moduleAddresses[0], PAGESIZE);
  sti();
	((EntryPoint)EXEC_MEM_ADDR)(0,0);
}

void copyAndExecuteModule(int moduleIndex, int argc, char *argv[]){
	memcpy(EXEC_MEM_ADDR, moduleAddresses[moduleIndex], PAGESIZE);
	sti();
	((EntryPoint)EXEC_MEM_ADDR)(argc, argv);
 	copyAndExecuteDefaultModule();
}

void * getFreePage() {
	static uint64_t last = EXEC_MEM_ADDR;
	last += PAGESIZE;
	return (void *)last;
}

void * getStackBase()
{
	return (void*)(
		MAPPEDMEMORY						//Stack starts at the end of the virtual memory
		- sizeof(uint64_t)      //Begin at the top of the stack
	);
}

context_t * createContext(uint64_t dataPageAddress, uint64_t heapPageAddress, uint64_t heapSize) { //TODO: Support many heap pages
	if(heapPageAddress == NULL) {
		heapPageAddress = (uint64_t)getFreePage();
		heapSize = EMPTY;
	}

	uint64_t stackPageAddress = (uint64_t)getFreePage();

	context_t * newContext = malloc(sizeof(context_t));
	newContext->dataPage.index = EXEC_MEM_ADDR/PAGESIZE;
	newContext->dataPage.address = dataPageAddress;
	newContext->heapPage.index = USER_HEAP/PAGESIZE;
	newContext->heapPage.address = heapPageAddress;
	newContext->stackPage.index = STACKBASE/PAGESIZE;
	newContext->stackPage.address = stackPageAddress;
	newContext->heapBase = USER_HEAP;
	newContext->heapSize = heapSize;

	return newContext;
}

void buildThreadStack(uint64_t rdi, uint64_t rsi, uint64_t rip, context_t * threadContext) {
	void * from = buildStack(rdi, rsi, rip, STACKBASE);

	threadContext->interruptStack = malloc(INT_STACK_SIZE);
	memcpy(threadContext->interruptStack, from, INT_STACK_SIZE);
}

context_t * createFirstThreadContext(int moduleIndex, int argc, char *argv[]) {
	void * dataPageAddress = getFreePage();
	//ChangePDE
	memcpy(dataPageAddress, moduleAddresses[moduleIndex], PAGESIZE);
	context_t * newContext = createContext(dataPageAddress, NULL, NULL);
	buildThreadStack((uint64_t)argc, (uint64_t)argv, EXEC_MEM_ADDR, newContext);
	return newContext;
}

context_t * createThreadContext(context_t * siblingContext, void * start_routine, void * arg) {
	context_t * newContext = createContext(siblingContext->dataPage.address, siblingContext->heapPage.address, siblingContext->heapSize);
	buildThreadStack((uint64_t)arg, NULL, (uint64_t)start_routine, newContext);
	return newContext;
}

void loadPage(page_t page) {
	changePDE(page.index, page.address, PRESENT);
}

void kernelMode() {
	context = kernelContext; //Just affects the heap
}

void userMode() {
	context = processContext; //Just affects the heap
}

void setContext(context_t * newContext) {
	if(processContext != NULL)
		saveContext();

	processContext = newContext;

	loadContext();
}

void saveContext() {
	memcpy(processContext->interruptStack, KERNEL_STACKBASE, INT_STACK_SIZE); //TODO: Is it set the first time?
}

void loadContext() {
	loadPage(processContext->dataPage);
	loadPage(processContext->heapPage);
	loadPage(processContext->stackPage);
	memcpy(KERNEL_STACKBASE, processContext->interruptStack, INT_STACK_SIZE);
}

void * malloc(uint64_t request) {

	uint64_t futureSize = context->heapSize + request;
	if (futureSize > PAGESIZE) {	//TODO: Add a page to the heap
		return NULL;
	}

	uint64_t blockAddress = context->heapBase + context->heapSize;
	context->heapSize = futureSize;

	return blockAddress;
}

void * realloc(void * ptr, uint64_t size) {
	void *  newptr = malloc(size);
	memcpy(newptr, ptr, size);
	return newptr;
}

void free(void * ptr) {

}

//LEGACY
void setKernelPresent(int present){
	uint64_t *PD = 0x10000;
	uint64_t entry= *PD;
	if(present)
		*PD = entry | 0x8F;
	else
		*PD = entry & ~0x1;
}

//TODO: Refactor
void changePDEPresent(int entry, int present){
	uint64_t PD = 0x10000;

	while(entry){
		PD += 8;
		--entry;
	}
	uint64_t PDE = *((uint64_t*)PD);

	if(present)
		*((uint64_t*)PD) =  PDE | 0x8F;
	else
		*((uint64_t*)PD) = PDE & ~0x1;
}

//TODO: Refactor
void changePDE(int entry, uint64_t physAddr, int present){
	if(physAddr & 0x001FFFFF != 0)
		return;


	uint64_t PD = PD_ADDR;

	while(entry){
		PD += 8;
		--entry;
	}


	if(present)
		*((uint64_t*)PD) = (uint64_t)physAddr | 0x8F;
	else
		*((uint64_t*)PD) = ((uint64_t)physAddr & ~(uint64_t)0x1FFFFF) & ~(uint64_t)0x1;
}

void pageFaultHandler(){ //TODO
	//Expand stack???
	print("PAGEFAULT");
	newLine();
	copyAndExecuteDefaultModule();
	//hang();
}

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void initializeKernelContext(uint64_t heapSize) {
	kernelContext = (context_t *) malloc(sizeof(context_t));
	kernelContext->heapSize = heapSize + sizeof(context_t);
	kernelContext->heapBase = KERNEL_HEAP;

	context = kernelContext;
}

void * initializeKernelBinary()
{
	ncPrint("Initializing Kernel...\n");

	int heapPage = KERNEL_HEAP/PAGESIZE;
	uint64_t heapPhyAddress = (uint64_t)getFreePage();
	changePDE(heapPage, heapPhyAddress, PRESENT); //Initialize Heap

	context_t tempContext;
	tempContext.heapBase = KERNEL_HEAP;
	tempContext.heapSize = EMPTY;
	context = &tempContext; //So module loader has a working heap

	void ** modules = loadModules(&endOfKernelBinary);
	clearBSS(&bss, &endOfKernel - &bss);
	moduleAddresses = modules; //AFTER BSS CLEAR TODO: Use avoid bss

	//changePDEPresent(40, NOT_PRESENT); //Just for PageFault Testing

	int stackPage = KERNEL_STACK / PAGESIZE;
	uint64_t stackPhyAddress = (uint64_t)getFreePage();
	changePDE(stackPage, stackPhyAddress, PRESENT); // Initialize Stack

	initializeKernelContext(tempContext.heapSize);
	processContext = NULL;

	return getStackBase();
}
