#include <stdint.h>
#include <moduleLoader.h>
#include <lib.h>
#include <terminal.h>

static const uint64_t PageSize = 0x200000;
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;


typedef int (*EntryPoint)(int argc, char *argv[]);

static void * const executableMemoryAdress = (void*)0x400000;
static void * const executableMemoryEndAdress = (void*)0x5FFFFF;
static void * const heapAddress = (void*)0x600000;

char* moduleNames[] = {"shell", "sampleDataModule", "sampleCodeModule", "hello", "help", "date", "time", "clear", "roflmao",0};
void * moduleAddresses[] = {0x0A00000, 0x0C00000, 0x0E00000, 0x1000000, 0x1200000, 0x1400000, 0x1600000, 0x1800000, 0x2000000};


void copyAndExectueDefaultModule(){
	memcpy(executableMemoryAdress, moduleAddresses[0], 0x200000);
  sti();
	((EntryPoint)executableMemoryAdress)(0,0);
}
void copyAndExecuteModule(int moduleIndex, int argc, char *argv[]){
	memcpy(executableMemoryAdress, moduleAddresses[moduleIndex], 0x200000);
  sti();
	((EntryPoint)executableMemoryAdress)(argc, argv);
  copyAndExectueDefaultModule();
}

void * malloc(uint64_t request) {
  static uint64_t capacity = 0x200000;
  static uint64_t size = 0;
  uint64_t futureSize = size + request;
  if(futureSize > capacity)
    return 0;

  uint64_t blockAddress = heapAddress + size;
  size = futureSize;
  return blockAddress;
}

char** backupArguments(int argc, char * argv[]) {
  if(argc > 0) {
    if(argv >= executableMemoryAdress && argv < executableMemoryEndAdress) {
      char ** temp = malloc(argc*sizeof(char **));
      if(temp == 0)
        return argv;
      memcpy(temp, argv, argc*sizeof(char **));
      argv = temp;
    }
    for(int i = 0; i < argc; i++) {
      size_t len = strlen(argv[i]) + 1;
      char * temp = malloc(len*sizeof(char));
      if(temp == 0)
        break;
      memcpy(temp, argv[i], len*sizeof(char));
      argv[i] = temp;
    }
  }
  return argv;
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
	uint64_t* PD = 0x10000;

	while(entry){ 
    PD += 0x8; 
    --entry; 
  } 
 	uint64_t PDE = *PD;
 
 	if(present)
  		*PD =  PDE | 0x8F; 
  	else
  		*PD = PDE & 0xFE;  
}

void changePDE(int entry, uint64_t* physAddr, int present){ 
  if(*physAddr & 0x000FFFFF != 0) 
    return; 
 
  
  uint64_t *PD = 0x10000; 
 
  while(entry){ 
    PD += 0x8; 
    --entry; 
  } 

  uint64_t oldEntry = *PD;
 
  if(present) 
    *PD = oldEntry & 0xFFFFF | (uint64_t)physAddr & ~0xFFFFF | 0x8F; 
  else 
    *PD = oldEntry & 0xFFFFF | (uint64_t)physAddr & ~0xFFFFF & ~0x1; 
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
    (uint64_t)&endOfKernel
    + PageSize * 8        //The size of the stack itself, 32KiB
    - sizeof(uint64_t)      //Begin at the top of the stack
  );
}

void * initializeKernelBinary()
{

  loadModules(&endOfKernelBinary, moduleAddresses);
  clearBSS(&bss, &endOfKernel - &bss);
  changePDEPresent(4, 0); //Empty page between heap and modules
  return getStackBase();
}
