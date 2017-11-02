#include <PCBTableADT.h>
#include <lib.h>

typedef struct pcbCDT{
	int pid;
	int privilege;
	int state;
	int childrenCount;
	int children[MAX_CHILDREN];
	//uint64_t programCounter;
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
	//new->programCounter=dir;
	//new->stack=dir;
	return newPCB->pid;
}

void removePCB(int id){
	int found = FALSE;
	for (int i = 0; i < tableSize || found; ++i){
		if(pcbTable[i]->pid == id){
			//free de pcbTable[i]
			pcbTable[i] = NULL;
			found = TRUE;
		}
	}
}

void changeState(int id, int state){
	int found = FALSE;
	for (int i = 0; i < tableSize || found; ++i){
		if(pcbTable[i]->pid == id){
			pcbTable[i]->state = state;
			found = TRUE;
		}
	}
}

void addChild(int fatherId){
	//pcbADT pcbTable = pcbTable;
	int found = FALSE;
	for (int i = 0; i < tableSize || found; ++i){
		if(pcbTable[i]->pid == fatherId){
			createChild(pcbTable[i]);
			found = TRUE;
		}
	}
}

void createChild(pcbADT father){
	int childId = addPCB(father->privilege);//no se que poner
	father->children[father->childrenCount] = childId;
	(father->childrenCount)++;
}



