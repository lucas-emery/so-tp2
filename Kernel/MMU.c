#include <stdint.h>
#include <moduleLoader.h>
#include <lib.h>
#include <naiveConsole.h>
#include <terminal.h>
#include <MMU.h>
#include <heap.h>
#include <pageAllocator.h>
#include <process.h>

#define GDT_ADDR 0x1000
#define TSS_ADDR 0x50000 //Free space based on Pure64 Manual
#define TSS_LIMIT 0x1000
#define KERNEL_CS 0x8
#define USER_CS 0x18
#define TSS_SEL 0x28
#define PD_ADDR 0x10000

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern void hang();
extern void loadGDTR(uint32_t address, uint16_t limit);
extern void loadTR(uint16_t tr);


char* moduleNames[] = {"shell", "sampleDataModule", "ts", "hello", "help", "date", "time", "clear", "roflmao","ps", "ls", "philosophers","multi","sem","echo","kill","accessKernel","man", "prodcons", 0};
void ** moduleAddresses = AVOID_BSS;
uint32_t moduleCount = AVOID_BSS;


void loadPage(page_t page) {
	changePDE(page.index, page.address, PRESENT);
}

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


void changePDE(int entry, uint64_t physAddr, int present){
	if(physAddr & 0x001FFFFF != 0)
		return;

	uint64_t PD = PD_ADDR;

	while(entry){
		PD += 8;
		--entry;
	}

	if(present)
		*((uint64_t*)PD) = (uint64_t)physAddr | 0x8F; //TODO: Don´t override
	else
		*((uint64_t*)PD) = ((uint64_t)physAddr | 0x8F) & ~(uint64_t)0x1;
}

void pageFaultHandler(){
	kernelMode();
	print("ACCESS DENIED");
	newLine();
	exitCurrentProcess(0xE);
	while(1);
}

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void initStack() {
	changePDE(KERNEL_STACK/PAGESIZE, getFreePage(), PRESENT);
	flushPaging();
}

void * initializeKernelBinary()
{
	ncPrint("Initializing Kernel...\n");

	initKernelContext();

	initPageAllocator();

	loadModules(&endOfKernelBinary);

	clearBSS(&bss, &endOfKernel - &bss);

	initStack();

	return KERNEL_STACKBASE;
}

void enableMemoryProtection() {
	changePDEPrivilege(KERNEL/PAGESIZE, SUPERVISOR);
	changePDEPrivilege(KERNEL_HEAP/PAGESIZE, SUPERVISOR);
	changePDEPrivilege(SYSCALL_STACK/PAGESIZE, SUPERVISOR);
	changePDEPrivilege(KERNEL_STACK/PAGESIZE, SUPERVISOR);

	int i = 0;
	while(i < moduleCount) {
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

	GDT[3] = create_descriptor(0x0,0xFFFFFFFF,0x20F8);
	GDT[4] = create_descriptor(0x0,0xFFFFFFFF,0x20F2);
	GDT[5] = create_lower_system_descriptor(TSS_ADDR, TSS_LIMIT, 0x2089);
	GDT[6] = create_upper_system_descriptor(TSS_ADDR);

	loadGDTR(GDT_ADDR, 7 * sizeof(uint64_t));
}

void setupTSS() {
	uint32_t * TSS = TSS_ADDR;
	TSS[1] = KERNEL_STACK_BOTTOM;				//Bits 31-0
	TSS[2] = KERNEL_STACK_BOTTOM >> 32;	//Bits 63-32
	TSS[9] = SYSCALL_STACK_BOTTOM;								//IST for SysCalls
	TSS[10] = SYSCALL_STACK_BOTTOM >> 32;
	TSS[11] = KERNEL_STACK_BOTTOM;
	TSS[12] = KERNEL_STACK_BOTTOM >> 32;

	loadTR(TSS_SEL);
}
