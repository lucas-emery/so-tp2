#include <stdint.h>
#include <moduleLoader.h>
#include <lib.h>
#include <terminal.h>
#include <naiveConsole.h>
#include <terminal.h>
#include <MMU.h>

#define GDT_ADDR 0x1000
#define TSS_ADDR 0x50000 //Free space based on Pure64 Manual
#define TSS_LIMIT 0x1000
#define KERNEL_CS 0x8
#define USER_CS 0x18
#define TR 0x28
#define PD_ADDR 0x10000
#define PAGESIZE 0x200000
#define KERNEL 0x0
#define KERNEL_HEAP 0x200000
#define KERNEL_STACK 0x400000
#define	CONTEXT_SWITCH_STACK 0x600000
#define SHARED_MEMORY 0x800000
#define EXEC_MEM_ADDR 0xA00000
#define ROM 0xC00000
#define MAPPEDMEMORY 0x100000000
#define USER_HEAP (MAPPEDMEMORY/2)
#define STACKBASE (MAPPEDMEMORY - sizeof(uint64_t))
#define KERNEL_STACKBASE (KERNEL_STACK + PAGESIZE - sizeof(uint64_t))
#define	CONTEXT_SWITCH_STACKBASE (CONTEXT_SWITCH_STACK + PAGESIZE - sizeof(uint64_t))
#define KERNEL_STACK_BOTTOM (KERNEL_STACK + PAGESIZE)
#define CS_STACK_BOTTOM (CONTEXT_SWITCH_STACK + PAGESIZE)
#define PRESENT 1
#define NOT_PRESENT 0
#define AVOID_BSS 1
#define INITIAL_PC 3
#define HEAP_PAGE_IDX 0
#define STACK_PAGE_IDX 1
#define DATA_PAGE_IDX 2
#define EMPTY 0
#define NULL 0
#define CS_STACK_SIZE (21 * 8)
#define SUPERVISOR 0
#define USER 1

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
extern void loadGDTR(uint32_t address, uint16_t limit);
extern void loadTR(uint16_t tr);
extern void flushPaging();

typedef int (*EntryPoint)(int argc, char *argv[]);

char* moduleNames[] = {"shell", "sampleDataModule", "sampleCodeModule", "hello", "help", "date", "time", "clear", "roflmao","ps", "ls", "philosophers","multi","sem",0};
void ** moduleAddresses;
static context_t * context = AVOID_BSS;
static context_t * kernelContext = AVOID_BSS;
static context_t * processContext = AVOID_BSS;
static context_t * sharedContext = AVOID_BSS;

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
	uint64_t kernelPageAddress = (uint64_t)getFreePage();

	context_t * newContext = malloc(sizeof(context_t));
	newContext->dataPage.index = EXEC_MEM_ADDR/PAGESIZE;
	newContext->dataPage.address = dataPageAddress;
	newContext->heapPage.index = USER_HEAP/PAGESIZE;
	newContext->heapPage.address = heapPageAddress;
	newContext->stackPage.index = STACKBASE/PAGESIZE;
	newContext->stackPage.address = stackPageAddress;
	newContext->kernelPage.index = KERNEL_STACK/PAGESIZE;
	newContext->kernelPage.address = kernelPageAddress;
	newContext->heapBase = USER_HEAP;
	newContext->heapSize = heapSize;

	return newContext;
}

void buildThreadStack(uint64_t rdi, uint64_t rsi, uint64_t rip, context_t * threadContext) {
	void * rsp = buildStack(rdi, rsi, rip, STACKBASE);

	threadContext->interruptContext = malloc(CS_STACK_SIZE);
	memcpy(threadContext->interruptContext, rsp, CS_STACK_SIZE);
	setStackPtr(rsp - CS_STACK_SIZE); //Clean stack
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
	loadPage(processContext->heapPage);
	flushPaging();
}

context_t * createFirstThreadContext(int moduleIndex, int argc, char *argv[]) {
	void * dataPageAddress = getFreePage();
	memcpy(dataPageAddress, moduleAddresses[moduleIndex], PAGESIZE);
	context_t * newContext = createContext(dataPageAddress, NULL, NULL);

	sharedMode();
	argv = moveArgsToActiveHeap(argc, argv);
	changeHeap(newContext);
	argv = moveArgsToActiveHeap(argc, argv);
	restoreProcessHeap();
	kernelMode();

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

void sharedMode() {
	context = sharedContext;
}

void setContext(context_t * newContext) {
	if(processContext != NULL)
		saveContext();

	processContext = newContext;

	loadContext();
	flushPaging();
}

void saveContext() {
	memcpy(processContext->interruptContext, CS_STACK_BOTTOM - CS_STACK_SIZE, CS_STACK_SIZE); //TODO: Is it set the first time?
}

void loadContext() {
	loadPage(processContext->dataPage);
	loadPage(processContext->heapPage);
	loadPage(processContext->stackPage);
	loadPage(processContext->kernelPage);
	memcpy(CS_STACK_BOTTOM - CS_STACK_SIZE, processContext->interruptContext, CS_STACK_SIZE);
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
	uint64_t PD = PD_ADDR;

	while(entry){
		PD += 8;
		--entry;
	}
	uint64_t PDE = *((uint64_t*)PD);

	if(present)
		*((uint64_t*)PD) =  PDE | 0x1;
	else
		*((uint64_t*)PD) = PDE & ~(uint64_t)0x1;
}

void changePDEPrivilege(int entry, int privilege) {
	uint64_t PD = PD_ADDR;

	while(entry){
		PD += 8;
		--entry;
	}
	uint64_t PDE = *((uint64_t*)PD);

	if(privilege == SUPERVISOR)
		*((uint64_t*)PD) =  PDE & ~(uint64_t)0x4;
	else
		*((uint64_t*)PD) = PDE | 0x4;
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
		*((uint64_t*)PD) = (uint64_t)physAddr & ~(uint64_t)0x1;
}

void pageFaultHandler(){ //TODO
	//Expand stack???
	print("PAGEFAULT");
	newLine();
	exitCurrentProcess(0xE);
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

void initSharedMemory() {
	sharedContext = (context_t *) malloc(sizeof(context_t));
	sharedContext->heapSize = EMPTY;
	sharedContext->heapBase = SHARED_MEMORY;
}

void * initializeKernelBinary()
{
	ncPrint("Initializing Kernel...\n");

	int heapPage = KERNEL_HEAP/PAGESIZE;				//TODO sacar esto
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

	//int stackPage = KERNEL_STACK / PAGESIZE;
	//uint64_t stackPhyAddress = (uint64_t)getFreePage();
	//changePDE(stackPage, stackPhyAddress, PRESENT); // Initialize Stack

	initializeKernelContext(tempContext.heapSize);
	processContext = NULL;

	return CONTEXT_SWITCH_STACKBASE;
}

void enableMemoryProtection() {
	changePDEPrivilege(KERNEL/PAGESIZE, SUPERVISOR);
	changePDEPrivilege(KERNEL_HEAP/PAGESIZE, SUPERVISOR);
	changePDEPrivilege(KERNEL_STACK/PAGESIZE, SUPERVISOR);
	changePDEPrivilege(CONTEXT_SWITCH_STACK/PAGESIZE, SUPERVISOR);

	int i = 0;
	while(moduleNames[i] != 0) {
		changePDEPrivilege((uint64_t)moduleAddresses[i]/PAGESIZE, SUPERVISOR);
		i++;
	}
}

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag){
    uint64_t descriptor;

    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24

    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;

    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0

    return descriptor;
}

uint64_t create_upper_system_descriptor(uint64_t base){
	uint64_t descriptor;

	descriptor = (base >> 32);         // set base bits 63:32

	return descriptor;
}

uint64_t create_lower_system_descriptor(uint64_t base, uint32_t limit, uint16_t flags){
	return create_descriptor((uint32_t)base, limit, flags);
}

void setupGDT(){
	uint64_t * GDT = GDT_ADDR;
	/*
	GDT[USER_CS >> 3] = create_descriptor(0x0,0xFFFFFFFF,0x20F8);
	GDT[TR >> 3] = create_lower_system_descriptor(TSS_ADDR, TSS_LIMIT, 0x0089);
	GDT[(TR >> 3) + 1] = create_upper_system_descriptor(TSS_ADDR);
	*/
	GDT[3] = create_descriptor(0x0,0xFFFFFFFF,0x20F8);
	GDT[4] = create_descriptor(0x0,0xFFFFFFFF,0x20F2);
	GDT[5] = create_lower_system_descriptor(TSS_ADDR, TSS_LIMIT, 0x2089);
	GDT[6] = create_upper_system_descriptor(TSS_ADDR);

	loadGDTR(GDT_ADDR, 7 * sizeof(uint64_t));
}

void setupTSS() {
	uint32_t * TSS = TSS_ADDR;
	TSS[1] = CS_STACK_BOTTOM;				//Bits 31-0
	TSS[2] = CS_STACK_BOTTOM >> 32;	//Bits 63-32
	TSS[9] = KERNEL_STACK_BOTTOM;								//IST for SysCalls
	TSS[10] = KERNEL_STACK_BOTTOM >> 32;
	TSS[11] = CS_STACK_BOTTOM;
	TSS[12] = CS_STACK_BOTTOM >> 32;

	loadTR(TR);
}
