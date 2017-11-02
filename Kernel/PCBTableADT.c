#include <PCBTableADT.h>
#include <lib.h>

typedef struct pcbCDT{
	int pid;
	int privilege;
	int state;
	int childrenCount;
	int children[MAX_CHILDREN];
	uint64_t stack;
};

int addPCB(int privilege){
	pcbADT newPCB;//reservar memoria
	pcbTable[tableSize] = newPCB;
	newPCB->pid = idCount;
	idCount++;
	tableSize++;
	newPCB->state = READY;
	newPCB->privilege = privilege;
	newPCB->childrenCount = 0;
	//newPCB->stack=dir;
	return newPCB->pid;
}

void removePCB(int id){
	int found = FALSE;
	for (int i = 0; i < tableSize || found; ++i){
		if(pcbTable[i]->pid == id){
			//free de pcbTable[i] y stack
			pcbTable[i] = NULL;
			found = TRUE;
		}
	}
}

void changeState(int id, int state){
	//hay que hacer las colas (dispatcher)
	int found = FALSE;
	for (int i = 0; i < tableSize || found; ++i){
		if(pcbTable[i]->pid == id){
			pcbTable[i]->state = state;
			found = TRUE;
		}
	}
}

int addChild(int fatherId){
	//pcbADT pcbTable = pcbTable;
	int childId;
	int found = FALSE;
	for (int i = 0; i < tableSize || found; ++i){
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
	/*strcat(buffer, "PID PRIVILEGE STATE\n");
	for (int i = 0; i < tableSize; ++i)
		strcat(buffer, makeString(pcbTable[i]));*/
} 

char* makeString(pcbADT process){
	char aux[100];
	char str[15];
	/*itoa(process->pid,str,10);
	strcat(aux, str);
	itoa(process->privilege,str,10);
	strcat(aux, str);
	if(process->state == RUNNING)
		strcpy(str, "running");
	else if(process->state == BLOCKED)
		strcpy(str, "blocked");
	else if(process->state == READY)
		strcpy(str, "ready");
	strcat(aux, str);
	strcat(aux, "\n");*/
	return aux;
}