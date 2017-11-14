#include <stdint.h>
#include <MMU.h>
#include <terminal.h>

#define FREE 0
#define USED 1
#define SYSTEM_RAM_ADDRESS 0x5020
#define MB 0x100000

static uint8_t * pages = AVOID_BSS;
static uint64_t size = AVOID_BSS;
static uint64_t firstCandidate = AVOID_BSS;

void initPageAllocator() {
  uint64_t i;
  uint32_t ram = *((uint32_t*)SYSTEM_RAM_ADDRESS);
  size = ram / (PAGESIZE / MB);
  pages = malloc(size * sizeof(uint8_t));
  firstCandidate = 0;
  for(i = 0; i < size; i++) {
    pages[i] = FREE;
  }
  pages[KERNEL/PAGESIZE] = USED;
  pages[KERNEL_HEAP/PAGESIZE] = USED;
  for(i = 0; i < MAPPEDMEMORY / PAGESIZE; i++) {
    if(i != KERNEL/PAGESIZE && i != KERNEL_HEAP/PAGESIZE) {
      changePDEPresent(i, NOT_PRESENT);
    }
  }
}


uint64_t getFreePage() {
  uint64_t index = firstCandidate;
  while (index < size && pages[index] == USED) {
    index++;
  }
  if(index == size) {
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
