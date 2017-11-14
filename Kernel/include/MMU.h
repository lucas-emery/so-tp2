#ifndef MMU_H
#define MMU_H

#include <context.h>
#include <heap.h>

#define PAGESIZE 0x200000
#define KERNEL 0x0
#define KERNEL_HEAP 0x200000
#define	KERNEL_STACK 0x400000
#define SYSCALL_STACK 0x600000
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
#define SUPERVISOR 0
#define USER 1
#define CONT_SW_STACK_SIZE (21 * 8)

void changePDE(int entry, uint64_t physAddr, int present);
void changePDEPresent(int entry, int present);
void * initializeKernelBinary();
void enableMemoryProtection();
void setupTSS();
void setupGDT();
void loadPage(page_t page);

extern uint64_t getStackPtr();
extern void setStackPtr(uint64_t rsp);
extern void * buildStack(uint64_t argc, uint64_t argv, uint64_t rip, uint64_t rsp);
extern uint64_t swapStack(uint64_t newStackPtr, int pageIndex, uint64_t pageAddress);
extern void flushPaging();

#endif
