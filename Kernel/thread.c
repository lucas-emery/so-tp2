#include <thread.h>

static int tid = 0;

tcbADT createFirstTCB(int privilege, int pid, int moduleIndex, int argc, char *argv[]){
	tcbADT newTCB =	malloc(sizeof(tcbCDT));
	newTCB->tid = tid++;
	newTCB->pid = pid;
	newTCB->privilage = privilege;
	newTCB->context = createFirstThreadContext(moduleIndex, argc, argv);
	newTCB->state = NEW;
	addThread(newTCB);
	return newTCB;
}

tcbADT createTCB(int privilege, void * startRoutine, void * arg){
	tcbADT newTCB =	malloc(sizeof(tcbCDT));
	newTCB->tid = tid++;
	newTCB->pid = getCurrentProcess();
	newTCB->privilage = privilege;
	newTCB->context = createThreadContext(getContextOfSibling(newTCB->pid), startRoutine, arg); //si marti devuelve null explota todo
	newTCB->state = NEW;
	addTCB(newTCB);
	addThread(newTCB);
	return newTCB;
}

int createThread(void *(*startRoutine)(void*), void * arg){
	tcbADT newThread = createTCB(3, startRoutine, arg);
	return newThread->tid;
}

char* makeStringThread(tcbADT thread, char* name, int maxNameLength){
	char* aux = malloc(100);
	char* str1 = malloc(10);
	char* str2 = malloc(10);
	char* str3 = malloc(10);
	char* str4 = malloc(10);
	int diff = maxNameLength - strlen(name);
	strcat(aux, "TID: ");
	uintToBase(thread->tid,str1,10);
	strcat(aux, str1);
	strcat(aux, " PID: ");
	uintToBase(thread->pid,str2,10);
	strcat(aux, str2);
	strcat(aux, " PROCESS NAME: ");
	strcat(aux, name);
	for(int i = 0; i < diff; i++)
		strcat(aux," ");
	strcat(aux, " PRIVILEGE: ");
	uintToBase(thread->privilage,str3,10);
	strcat(aux, str3);
	strcat(aux, " STATE: ");
	switch(thread->state){
		case RUNNING:
			strcpy(str4, "running\n");
		break;
		case BLOCKED:
			strcpy(str4, "blocked\n");
		break;
		case READY:
			strcpy(str4, "ready\n");
		break;
		case NEW:
			strcpy(str4, "new\n");
		break;
		case DEAD:
			strcpy(str4, "dead\n");
		break;
	}
	strcat(aux, str4);
	return aux;
}
