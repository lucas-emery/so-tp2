#include <process.h>
#include <terminal.h>

extern char* moduleNames[];
int exitValue;

int createProcess(int moduleIndex, int argc, char * argv[]){
	int newProcessID = addPCB(moduleNames[moduleIndex], 3); // default privilege 3
	tcbADT newProcessThread = createFirstTCB(3, newProcessID, moduleIndex, argc, argv); // default privilege 3
	addTCB(newProcessThread);
	return newProcessID;
}

void exitProcess(int pid, int value) {
	removePCB(pid);
	if(pid == getFocusedPID()) {
		setFocusedPID(createProcess(0,0,0));
	}
	exitValue = value;
	if(pid == getCurrentProcess())
		intTT();
}

void exitCurrentProcess(int value) {
	exitProcess(getCurrentProcess(), value);
}
