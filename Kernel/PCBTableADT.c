#include <PCBTableADT.h>
#include <lib.h>

typedef struct {
	int pid;
	int privilege;
	int state;
	int childrenCount;
	int children[MAX_CHILDREN];
	//uint64_t programCounter;
	uint64_t stack;
} pcbCDT;

int addPCB(int privilege){
	pcbADT new;//reservar memoria
	pcbTable[size] = new;
	new->pid = idCount;
	idCount++;
	tableSize++;
	new->state = READY;
	new->privilege = privilege;
	new->next = NULL;
	new->childrenCount = 0;
	//new->programCounter=dir;
	//new->stack=dir;
	if(last == NULL)
		last = new;
	else
		last->next = new;
	return new->pid;
}

void removePCB(int id){
	pcbTable = remove(pcbTable, id);
}

pcbADT remove(pcbADT pcbTable[i], int id){
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
	pcbADT pcbTable[i] = pcbTable;
	int found = FALSE;
	for (int i = 0; i < tableSize || found; ++i){
		if(pcbTable[i]->pid == id){
			createChild(pcbTable[i]);
			found = TRUE;
		}
	}
}

void createChild(pcbADT father){
	int childId = addPCB(father->privilege);//no se que poner
	father->children[childrenCount] = childId;
	father->childrenCount++;
}



