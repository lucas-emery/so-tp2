#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interruptions.h>
#include <terminal.h>
#include <MMU.h>
#include <pcb.h>
#include <scheduler.h>

int main(){
	clearScreen();
	setupGDT();
	setupTSS();
	setupIDT();
	setupPCB();
	initScheduler();
	initSharedMemory();
	enableMemoryProtection();
	ncClear();
	createProcess(0,0,0);
  sti();
  while(1);
}
