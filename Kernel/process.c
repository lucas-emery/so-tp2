#include <process.h>

int createProcess(char * processInfo){
	int argc;
	char argv[10][100];
	int moduleIndex;
	char processName[100];
	int i = 0;
	int j = 0;
	int k = 0;

	while(processInfo[i] != ' ' && processInfo[i] != 0){
		processName[j] = processInfo[i];
		i++;
		j++;

	}
	processName[j] = 0;

	if(processInfo[i] != 0)
			i++;
	j=0;
	while(processInfo[i] != 0){
		while(processInfo[i] != ' ' && processInfo[i] != 0){
			argv[k][j] = processInfo[i];
			i++;
			j++;
		}
		k++;
		if(processInfo[i] != 0)
			i++;
	}
	argc = k;

	int found = 0;
	for(i=0; moduleNames[i] != 0 && !found; i++){
		if(strcmp(processName, moduleNames[i]) == 0)
			found = 1;
	}
	if(found)
		moduleIndex = i-1;
	else
		return -1;

	int newProcessID = addPCB(3, processName); // default privilege 3

	tcbADT newProcessThread = createFirstTCB(3, moduleIndex, argc, argv); // default privilege 3
	addTCB(newProcessThread);
	
	return newProcessID;
}