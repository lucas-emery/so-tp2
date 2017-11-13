#include <stdint.h>
#include <MMU.h>
#include <terminal.h>

#define FREE 0
#define USED 1

static uint8_t * pages = AVOID_BSS;
static uint64_t size = AVOID_BSS;
static uint64_t firstCandidate = AVOID_BSS;

void initPageAllocator() {
  size = MAPPEDMEMORY / PAGESIZE;
  pages = malloc(size * sizeof(uint8_t));
  firstCandidate = 0;
  for(uint64_t i = 0; i < size; i++) {
    pages[i] = FREE;
  }
  pages[KERNEL/PAGESIZE] = USED;
  pages[KERNEL_HEAP/PAGESIZE] = USED;
}


uint64_t getFreePage() {
  uint64_t index = firstCandidate;
  while (index < size && pages[index] == USED) {
    index++;
  }
  if(index == size) {
    //Kill process?
    print("OUT OF MEMORY :/");
    while(1);
  }
  pages[index] = USED;
  firstCandidate = index + 1;
  return index * PAGESIZE;
}

void freePage(uint64_t address) {
  uint64_t index = address / PAGESIZE;
  pages[index] = FREE;
  if(index < firstCandidate) {
    firstCandidate = index;
  }
}
