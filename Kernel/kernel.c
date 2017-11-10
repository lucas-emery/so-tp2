#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interruptions.h>
#include <terminal.h>
#include <MMU.h>

int main(){
	clearScreen();
	setupGDT();
	setupTSS();
	setupIDT();
	copyAndExecuteDefaultModule();
}
