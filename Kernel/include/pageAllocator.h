#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H

void initPageAllocator();
uint64_t getFreePage();
void freePage(uint64_t address);

#endif
