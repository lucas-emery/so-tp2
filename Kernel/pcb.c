#include <pcb.h>

typedef struct pcbCDT{
	char* name;
	int pid;
	int state;
	int privilege;
	tcbADT* threads;
	int threadCount;
}pcbCDT;

static int idCount = 0;
static pcbADT * pcbTable;
static int tableSize = 0;

/*
*Creates the string having the info of the process in parameter
*Returns the string created
*/
static char* makeString(pcbADT process);

void setupPCB(){
	pcbTable = malloc(sizeof(pcbADT));
}

int addPCB(char * name, int privilege){
	pcbADT newPCB = malloc(sizeof(pcbCDT));
	newPCB->pid = idCount++;
	newPCB->state = NEW;
	newPCB->name = name;
	newPCB->privilege = privilege;
	tableSize++;
	pcbTable = realloc(pcbTable, tableSize * sizeof(pcbADT));
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

void processesInfo(char* buffer){
	*buffer = 0;
	strcat(buffer, "NAME PID STATE PRIVILEGE\n");
	for (int i = 0; i < tableSize; i++){
		strcat(buffer, makeString(pcbTable[i]));
	}
}

static char* makeString(pcbADT process){
	char aux[100] = {0};
	char str[15];
	uintToBase(process->pid,str,10);
	strcat(aux, str);
	strcat(aux, process->name);
	switch(process->state){
		case RUNNING:
			strcpy(str, "running");
		break;
		case BLOCKED:
			strcpy(str, "blocked");
		break;
		case READY:
			strcpy(str, "ready");
		break;
		case NEW:
			strcpy(str, "new");
		break;
	}
	strcat(aux, str);
	uintToBase(process->privilege,str,10);
	strcat(aux, str);
	strcat(aux, "\n");
	return aux;
}

void addTCB(tcbADT tcb){
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == tcb->pid){
			pcbTable[i]->threadCount++;
			pcbTable[i]->threads = realloc(pcbTable[i]->threads, pcbTable[i]->threadCount * sizeof(tcbADT));
			pcbTable[i]->threads[pcbTable[i]->threadCount-1] = tcb;
			return;
		}
	}
}

context_t* getContextOfSibling(int id){
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == id){
			return (pcbTable[i]->threads)[0]->context;
		}
	}
	return NULL;
}

tcbADT getThreads(int id, int* count){
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == id){
			*count = pcbTable[i]->threadCount;
			return pcbTable[i]->threads;
		}
	}
	return NULL;
}
