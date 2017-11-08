#include <process.h>

typedef struct pcbCDT{
	int pid;
	int privilege;
	int state;
	int childrenCount;
	int children[MAX_CHILDREN];
	//TCB* threads;
	//int threadCount;
}pcbCDT;

static int idCount = 0;
static pcbADT* pcbTable;
static int tableSize = 0;

/*
*Creates the new id for the child and puts it in the father's PCB
*Returns the new child id
*/
static int createChild(pcbADT father);

/*
*Creates the string having the info of the process in parameter
*Returns the string created
*/
static char* makeString(pcbADT process);

void createTable(){
	pcbTable = malloc(sizeof(pcbADT));
}

int addPCB(int privilege){
	pcbADT newPCB;
	newPCB->pid = idCount++;
	newPCB->state = NEW;
	newPCB->privilege = privilege;
	newPCB->childrenCount = 0;
	tableSize++;
	realloc(pcbTable, tableSize * sizeof(pcbADT));
	pcbTable[tableSize-1] = newPCB;
	//*threads=firstTCB(privilege);
	return newPCB->pid;
}

int removePCB(int id){
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == id){
			free(pcbTable[i]);
			for(int j = i; j < tableSize-1; j++)
				pcbTable[j] = pcbTable[j+1];
			tableSize--;
			return 0;
		}
	}
	return 1;
}

int changeState(int id, int state){
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == id){
			pcbTable[i]->state = state;
			return 0;
		}
	}
	return 1;
}

int getState(int id){
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == id)
			return pcbTable[i]->state;
	}
	return -1;
}

int addChild(int fatherId){
	int childId;
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == fatherId){
			childId = createChild(pcbTable[i]);
			return childId;
		}
	}
	return -1;
}

static int createChild(pcbADT father){
	int childId = addPCB(father->privilege);
	father->children[father->childrenCount] = childId;
	(father->childrenCount)++;
	return childId;
}

void processesInfo(char* buffer){
	*buffer = 0;
	strcat(buffer, "PID PRIVILEGE STATE\n");
	for (int i = 0; i < tableSize; i++){
		strcat(buffer, makeString(pcbTable[i]));
	}
} 

static char* makeString(pcbADT process){
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

/*void addTCB(TCB tcb){
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == tcb->pid){
			threadCount++;
			realloc(TCB, threadCount * sizeof(TCB));
			threads[threadCount-1] = tcb;
			return;
		}
	}
}

context_t getContextOfSibling(int id){
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == tcb->pid){
			return *threads->context;
		}
	}
	return NULL;
}

TCB getThreads(int id, int* count){
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == id){
			*count = pcbTable[i]->threadCount;
			return pcbTable[i]->threads;
		}
	}
	return NULL;
}*/