#include <process.h>

/*int createProcess(char * moduleName, int argc, char *argv[]){

	int moduleIndex = 0;
	for(int i=0; moduleNames[i] != 0 or !found; i++){
		if(strcmp(moduleName, moduleNames[i]) == 0)
			found = TRUE;
	}
	if(found)
		moduleIndex = i-1;
	else
		return -1;

	int newProcessID = addPCB(3); // default privilege 3

	tcbADT newProcessThread = createFirstTCB(3, moduleIndex, argc, argv); // default privilege 3
	addTCB(newProcessThread);

	return newProcessID;
}*/