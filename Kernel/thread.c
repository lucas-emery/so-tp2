#include <thread.h>

static int tid = 0;

tcbADT createFirstTCB(int privilege, int pid, int moduleIndex, int argc, char *argv[]){
	tcbADT newTCB =	malloc(sizeof(tcbCDT));
	newTCB->tid = tid++;
	newTCB->pid = pid;
	newTCB->context = createFirstThreadContext(moduleIndex, argc, argv);
	newTCB->state = NEW;
	addThread(newTCB);
	return newTCB;
}

tcbADT createTCB(int privilege, void * startRoutine, void * arg){
	tcbADT newTCB =	malloc(sizeof(tcbCDT));
	newTCB->tid = tid++;
	newTCB->pid = getCurrentProcess();
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
