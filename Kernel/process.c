#include <process.h>
#include <lib.h>
#include <MMU.h>

typedef struct pcbCDT{
	int pid;
	int privilege;
	int state;
	int childrenCount;
	int children[MAX_CHILDREN];
};

void createTable(){
	pcbTable = malloc(sizeof(pcbADT));
}

int addPCB(int privilege){
	pcbADT newPCB;
	newPCB->pid = idCount;
	idCount++;
	newPCB->state = NEW;//llamar a dispatcher y que ponga en cola
	newPCB->privilege = privilege;
	newPCB->childrenCount = 0;
	tableSize++;
	realloc(pcbTable, tableSize * sizeof(pcbADT));
	pcbTable[tableSize-1] = newPCB;
	return newPCB->pid;
}

void removePCB(int id){
	int found = FALSE;
	for (int i = 0; i < tableSize || !found; ++i){
		if(pcbTable[i]->pid == id){
			free(pcbTable[i]);
			for(int j = i; j < tableSize-1; j++)
				pcbTable[j] = pcbTable[j+1];
			tableSize--;
			found = TRUE;
		}
	}
}

void changeState(int id, int state){
	int found = FALSE;
	for (int i = 0; i < tableSize || !found; ++i){
		if(pcbTable[i]->pid == id){
			pcbTable[i]->state = state;
			found = TRUE;
		}
	}
}

int getState(int id){
	for (int i = 0; i < tableSize; ++i){
		if(pcbTable[i]->pid == id)
			return pcbTable[i]->state;
	}
	return -1;
}

int addChild(int fatherId){
	int childId;
	int found = FALSE;
	for (int i = 0; i < tableSize || !found; ++i){
		if(pcbTable[i]->pid == fatherId){
			childId = createChild(pcbTable[i]);
			found = TRUE;
		}
	}
	return childId;
}

int createChild(pcbADT father){
	int childId = addPCB(father->privilege);//no se que poner
	father->children[father->childrenCount] = childId;
	(father->childrenCount)++;
	return childId;
}

void processesInfo(char* buffer){
	*buffer = 0;
	strcat(buffer, "PID PRIVILEGE STATE\n");
	for (int i = 0; i < tableSize; ++i){
		strcat(buffer, makeString(pcbTable[i]));
	}
} 

char* makeString(pcbADT process){
	char aux[100] = {0};
	char str[15];
	uintToBase(process->pid,str,10);
	strcat(aux, str);
	uintToBase(process->privilege,str,10);
	strcat(aux, str);
	if(process->state == RUNNING)
		strcpy(str, "running");
	else if(process->state == BLOCKED)
		strcpy(str, "blocked");
	else if(process->state == READY)
		strcpy(str, "ready");
	strcat(aux, str);
	strcat(aux, "\n");
	return aux;
}