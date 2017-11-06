#include <stdint.h>
#include <moduleLoader.h>
#include <lib.h>
#include <terminal.h>
#include <naiveConsole.h>
#include <terminal.h>
#include <MMU.h>

#define PAGESIZE 0x200000
#define MAPPEDMEMORY 0x100000000
#define HEAPBASE (MAPPEDMEMORY/2)
#define STACKBASE (MAPPEDMEMORY - sizeof(uint64_t))
#define EXEC_MEM_ADDR 0x400000
#define ROM 0x600000
#define PRESENT 1
#define NOT_PRESENT 0
#define AVOID_BSS 1
#define INITIAL_PC 3
#define HEAP_PAGE_IDX 0
#define STACK_PAGE_IDX 1
#define DATA_PAGE_IDX 2
#define EMPTY 0

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern void hang();
extern uint64_t getStackPtr();
extern void setStackPtr(uint64_t rsp);
extern void buildStack(int argc, char * argv[], uint64_t rsp);

typedef int (*EntryPoint)(int argc, char *argv[]);

char* moduleNames[] = {"shell", "sampleDataModule", "sampleCodeModule", "hello", "help", "date", "time", "clear", "roflmao",0};
void ** moduleAddresses;
context_t * context = AVOID_BSS;
context_t * kernelContext;


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

context_t * createContext(uint64_t dataPageAddress) {
	void * heapPageAddress = getFreePage();
	void * stackPageAddress = getFreePage();

	context_t * newContext = malloc(sizeof(context_t));
	newContext->pages = malloc(INITIAL_PC * sizeof(page_t));
	newContext->pages[DATA_PAGE_IDX].index = EXEC_MEM_ADDR/PAGESIZE;
	newContext->pages[DATA_PAGE_IDX].address = (uint64_t)dataPageAddress;
	newContext->pages[HEAP_PAGE_IDX].index = HEAPBASE/PAGESIZE;
	newContext->pages[HEAP_PAGE_IDX].address = (uint64_t)heapPageAddress;
	newContext->pages[STACK_PAGE_IDX].index = STACKBASE/PAGESIZE;
	newContext->pages[STACK_PAGE_IDX].address = (uint64_t)stackPageAddress;
	newContext->pageCount = INITIAL_PC;
	newContext->heapSize = EMPTY;
	newContext->heapCapacity = PAGESIZE;
	newContext->stackPtr = STACKBASE;

	return newContext;
}

void buildThreadStack(int argc, char * argv[], context_t * threadContext) {
	changePDE(threadContext->pages[STACK_PAGE_IDX].index, threadContext->pages[STACK_PAGE_IDX].address, PRESENT);
	buildStack(argc, argv, threadContext->stackPtr);
	changePDE(context->pages[STACK_PAGE_IDX].index, context->pages[STACK_PAGE_IDX].address, PRESENT);
}

context_t * createFirstThreadContext(int moduleIndex, int argc, char *argv[]) {
	void * dataPageAddress = getFreePage();
	memcpy(dataPageAddress, moduleAddresses[moduleIndex], PAGESIZE);
	context_t * newContext = createContext(dataPageAddress);
	buildThreadStack(argc, argv, newContext);
	return newContext;
}

context_t * createThreadContext(context_t * siblingContext) {
	context_t * newContext = createContext(siblingContext->pages[DATA_PAGE_IDX].address);
	buildThreadStack(0, 0, newContext);
	return newContext;
}

//Mepa que va a quedar ree distinto, no diseñen nada en base a esto
void switchContext(context_t newContext) {
	//WIP
}

void * malloc(uint64_t request) {

  uint64_t futureSize = context->heapSize + request;
  while (futureSize > context->heapCapacity) {	//Add a page to the heap
		changePDE((HEAPBASE + context->heapCapacity) / PAGESIZE, (uint64_t)getFreePage(), 1);
		context->heapCapacity += PAGESIZE;
		//TODO: Add page to context
	}

  uint64_t blockAddress = HEAPBASE + context->heapSize;
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

void setKernelPresent(int present){
  uint64_t *PD = 0x10000;
  uint64_t entry= *PD;
  if(present)
    *PD = entry | 0x8F;
  else
    *PD = entry & ~0x1;
}

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

void changePDE(int entry, uint64_t physAddr, int present){
  if(physAddr & 0x001FFFFF != 0)
    return;


  uint64_t PD = 0x10000;

  while(entry){
    PD += 8;
    --entry;
  }


  if(present)
    *((uint64_t*)PD) = (uint64_t)physAddr | 0x8F;
  else
    *((uint64_t*)PD) = ((uint64_t)physAddr & ~(uint64_t)0x1FFFFF) & ~(uint64_t)0x1;
}

void pageFaultHandler(){
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

void initializeKernelContext(int stackPage, uint64_t stackPhyAddress, int heapPage, uint64_t heapPhyAddress, uint64_t heapCapacity, uint64_t heapSize) {
	context = (context_t *) malloc(sizeof(context_t));
	context->heapCapacity = heapCapacity;
	context->heapSize = heapSize + sizeof(context_t);
	context->pages = (page_t *) malloc(sizeof(page_t) * 2);
	context->pages[STACK_PAGE_IDX].index = stackPage;
	context->pages[STACK_PAGE_IDX].address = stackPhyAddress;
	context->pages[HEAP_PAGE_IDX].index = heapPage;
	context->pages[HEAP_PAGE_IDX].address = heapPhyAddress;
	context->pageCount = 2;
	context->stackPtr = (uint64_t)getStackBase();
}

void * initializeKernelBinary()
{
  ncPrint("Initializing Kernel...\n");

	int heapPage = HEAPBASE/PAGESIZE;
	uint64_t heapPhyAddress = (uint64_t)getFreePage();
	changePDE(heapPage, heapPhyAddress, PRESENT); //Initialize Heap

	context_t tempContext;
	tempContext.heapCapacity = PAGESIZE;
	tempContext.heapSize = EMPTY;
	context = &tempContext; //So module loader has a working heap

  void ** modules = loadModules(&endOfKernelBinary);
  clearBSS(&bss, &endOfKernel - &bss);
	moduleAddresses = modules; //AFTER BSS CLEAR

	changePDEPresent(40, NOT_PRESENT); //Just for PageFault Testing

	void * stackBase = getStackBase();
	int stackPage = (uint64_t)stackBase / PAGESIZE;
	uint64_t stackPhyAddress = (uint64_t)getFreePage();
	changePDE(stackPage, stackPhyAddress, PRESENT); // Initialize Stack

	initializeKernelContext(stackPage, stackPhyAddress, heapPage, heapPhyAddress, tempContext.heapCapacity, tempContext.heapSize);

  return stackBase;
}
