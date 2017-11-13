#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interruptions.h>
#include <terminal.h>
#include <MMU.h>
#include <pcb.h>
#include <scheduler.h>

void deb() {
	uint64_t * PD = 0x10000;
	for(int i = 0; i < 20; i++) {
		printHex(PD[i]);
		newLine();
	}
	while(1);
}

int main(){
	//while(1);
	clearScreen();
	setupGDT();
	setupTSS();
	setupIDT();
	setupPCB();
	initSharedMemory();
	//deb();
	initScheduler();
	//deb();
	enableMemoryProtection();
	setFocusedPID(createProcess(0,0,0));
	//deb();
  sti();
  while(1);
}
