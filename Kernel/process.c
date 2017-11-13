#include <process.h>
#include <terminal.h>

extern char* moduleNames[];
int exitValue;

int createProcess(int moduleIndex, int argc, char * argv[]){
	print("adding pcb ");
	int newProcessID = addPCB(moduleNames[moduleIndex], 3); // default privilege 3
	printDec(newProcessID);
	print(" creating thread ");

	tcbADT newProcessThread = createFirstTCB(3, newProcessID, moduleIndex, argc, argv); // default privilege 3
	print("addingtcb ");

	addTCB(newProcessThread);
	print("finished");
	newLine();

	return newProcessID;
}

void exitProcess(int pid, int value) {
	newLine();
	printDec(pid);
	newLine();
	printDec(getFocusedPID());
	newLine();

	removePCB(pid);
	if(pid == getFocusedPID()) {
		setFocusedPID(createProcess(0,0,0));
	}
	printDec(getFocusedPID());
	exitValue = value;
	intTT();
}

void exitCurrentProcess(int value) {
	exitProcess(getCurrentProcess(), value);
}

// int createProcess(char * processInfo){
// 	int argc;
// 	char argv[10][100];
// 	int moduleIndex;
// 	char processName[100];
// 	int i = 0;
// 	int j = 0;
// 	int k = 0;
//
//
// 	while(processInfo[i] != ' ' && processInfo[i] != 0){
// 		processName[j] = processInfo[i];
// 		i++;
// 		j++;
//
// 	}
// 	processName[j] = 0;
//
// 	if(processInfo[i] != 0)
// 			i++;
// 	j=0;
// 	while(processInfo[i] != 0){
// 		while(processInfo[i] != ' ' && processInfo[i] != 0){
// 			argv[k][j] = processInfo[i];
// 			i++;
// 			j++;
// 		}
// 		k++;
// 		if(processInfo[i] != 0)
// 			i++;
// 	}
// 	argc = k;
//
// 	int found = 0;
// 	for(i=0; moduleNames[i] != 0 && !found; i++){
// 		if(strcmp(processName, moduleNames[i]) == 0)
// 			found = 1;
// 	}
// 	if(found)
// 		moduleIndex = i-1;
// 	else
// 		return -1;
//
// 	int newProcessID = addPCB(3, processName); // default privilege 3
//
// 	//printDec(moduleIndex);
// 	//printHex(newProcessID);
//
// 	tcbADT newProcessThread = createFirstTCB(3, newProcessID, moduleIndex, argc, argv); // default privilege 3
// 	addTCB(newProcessThread);
//
//
// 	//printDec(removePCB(newProcessID));
//
// 	return newProcessID;
// }
