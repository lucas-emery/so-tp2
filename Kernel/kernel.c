#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interruptions.h>
#include <terminal.h>
#include <MMU.h>
#include <pcb.h>

int main(){
	clearScreen();
	cli();
	setupGDT();
	setupTSS();
	setupIDT();
	sti();
	initPCB();
	copyAndExecuteDefaultModule();
}
