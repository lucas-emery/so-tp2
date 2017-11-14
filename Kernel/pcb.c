#include <pcb.h>

typedef struct pcbCDT{
	char name[32];
	int pid;
	int state;
	int privilege;
	tcbADT* threads;
	int threadCount;
}pcbCDT;

static int idCount = 0;
static pcbADT * pcbTable;
static int tableSize = 0;
static int maxNameLength = 0;

/*
*Creates the string having the info of the process in parameter
*Returns the string created
*/
static char* makeStringProcess(pcbADT process);

void setupPCB(){
	pcbTable = malloc(sizeof(pcbADT));
}

int addPCB(char * name, int privilege){
	if(idCount == 0)
		name = "sysIdle";
	if(strlen(name) > maxNameLength)
		maxNameLength = strlen(name);
	pcbADT newPCB = malloc(sizeof(pcbCDT));
	newPCB->pid = idCount++;
	newPCB->state = NEW;
	strcat(newPCB->name,name);
	newPCB->privilege = privilege;
	tableSize++;
	pcbTable = realloc(pcbTable, tableSize * sizeof(pcbADT));
	pcbTable[tableSize-1] = newPCB;
	return newPCB->pid;
}

int removePCB(int id){
	for (int i = 0; i < tableSize; i++){
		if(pcbTable[i]->pid == id){
			pcbADT pcb = pcbTable[i];
			for(int j = 0; j < pcb->threadCount; j++)
				pcb->threads[j]->state = DEAD;
			freeProcessContext(pcb->threads[0]->context); //TODO: Existe este thread siempre?? Creo que si, gracias stu <3
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
	*buffer = NULL;
	for (int i = 0; i < tableSize; i++){
		strcat(buffer, makeStringProcess(pcbTable[i]));
	}
}

void threadsInfo(char* buffer){
	*buffer = NULL;
	for (int i = 0; i < tableSize; i++){
		for (int j = 0; j < pcbTable[i]->threadCount; j++){
			strcat(buffer, makeStringThread(pcbTable[i]->threads[j], pcbTable[i]->name, maxNameLength));	
		}
	}
}

static char* makeStringProcess(pcbADT process){
	char* aux = malloc(100);
	char* str1 = malloc(10);
	char* str2 = malloc(10);
	char* str3 = malloc(10);
	int diff = maxNameLength - strlen(process->name);
	strcat(aux, "PID: ");
	uintToBase(process->pid,str1,10);
	strcat(aux, str1);
	strcat(aux, " NAME: ");
	strcat(aux, process->name);
	for(int i = 0; i < diff; i++)
		strcat(aux," ");
	strcat(aux, " PRIVILEGE: ");
	uintToBase(process->privilege,str3,10);
	strcat(aux, str3);
	strcat(aux, " STATE: ");
	switch(process->state){
		case RUNNING:
			strcpy(str2, "running\n");
		break;
		case BLOCKED:
			strcpy(str2, "blocked\n");
		break;
		case READY:
			strcpy(str2, "ready\n");
		break;
		case NEW:
			strcpy(str2, "new\n");
		break;
	}
	strcat(aux, str2);
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
