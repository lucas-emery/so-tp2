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
	ncClear();
	cli();
	while(1) schedule();
}
