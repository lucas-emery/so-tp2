#include <stdint.h>
#include <moduleLoader.h>
#include <lib.h>
#include <terminal.h>
#include <naiveConsole.h>
#include <terminal.h>

#define PAGESIZE 0x200000
#define MAPPEDMEMORY 0x100000000
#define HEAPBASE (MAPPEDMEMORY/2)
#define EXEC_MEM_ADDR 0x400000

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;


typedef int (*EntryPoint)(int argc, char *argv[]);

//static void * const executableMemoryEndAdress = (void*)0x5FFFFF;
//static void * const heapAddress = (void*)0x600000;

char* moduleNames[] = {"shell", "sampleDataModule", "sampleCodeModule", "hello", "help", "date", "time", "clear", "roflmao",0};
void ** moduleAddresses;


void copyAndExectueDefaultModule(){
	memcpy(EXEC_MEM_ADDR, moduleAddresses[0], PAGESIZE);
  sti();
	((EntryPoint)EXEC_MEM_ADDR)(0,0);
}
void copyAndExecuteModule(int moduleIndex, int argc, char *argv[]){
	memcpy(EXEC_MEM_ADDR, moduleAddresses[moduleIndex], PAGESIZE);
  sti();
	((EntryPoint)EXEC_MEM_ADDR)(argc, argv);
  copyAndExectueDefaultModule();
}

void * malloc(uint64_t request) {
  static uint64_t capacity = PAGESIZE;
  static uint64_t size = 1; //Para que no quede en la bss
  uint64_t futureSize = size + request;
  if(futureSize > capacity)
    return 0;

  uint64_t blockAddress = HEAPBASE + size;
  size = futureSize;
  return blockAddress;
}

void * realloc(void * ptr, uint64_t size) {
	void *  newptr = malloc(size);
	memcpy(newptr, ptr, size);
	return newptr;
}

void free(void * ptr) {

}

// char** backupArguments(int argc, char * argv[]) {
//   if(argc > 0) {
//     if(argv >= EXEC_MEM_ADDR && argv < executableMemoryEndAdress) {
//       char ** temp = malloc(argc*sizeof(char **));
//       if(temp == 0)
//         return argv;
//       memcpy(temp, argv, argc*sizeof(char **));
//       argv = temp;
//     }
//     for(int i = 0; i < argc; i++) {
//       size_t len = strlen(argv[i]) + 1;
//       char * temp = malloc(len*sizeof(char));
//       if(temp == 0)
//         break;
//       memcpy(temp, argv[i], len*sizeof(char));
//       argv[i] = temp;
//     }
//   }
//   return argv;
// }

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
  copyAndExectueDefaultModule();
}

void clearBSS(void * bssAddress, uint64_t bssSize)
{
  memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
  return (void*)(
    MAPPEDMEMORY						//Stack starts at the end of the virtual memory
    - sizeof(uint64_t)      //Begin at the top of the stack
  );
}

void * getFreePage() {
	static uint64_t last = 0x800000;
	last += PAGESIZE;
	return (void *)last;
}

void * initializeKernelBinary()
{
  ncPrint("Initializing Kernel...\n");
	changePDE(HEAPBASE/PAGESIZE, (uint64_t)getFreePage(), 1); //TODO: Move inside malloc

  void ** modules = loadModules(&endOfKernelBinary);
  clearBSS(&bss, &endOfKernel - &bss);
	moduleAddresses = modules; //AFTER BSS CLEAR

  changePDEPresent(4, 0); //Empty page between heap and modules
	void * stackBase = getStackBase();
	changePDE((uint64_t)stackBase/PAGESIZE, (uint64_t)getFreePage(), 1);
  return stackBase;
}