#include <thread.h>

static int tid = 0;

tcbADT createFirstTCB(int privilege, int moduleIndex, int argc, char *argv[]){
	tcbADT newTCB =	malloc(sizeof(tcbADT));
	newTCB->tid = tid++;
	newTCB->pid = getCurrentProcess();
	newTCB->context = createFirstThreadContext(moduleIndex, argc, argv);
	newTCB->state = NEW;
	addThread(newTCB);
	return newTCB;
}

tcbADT createTCB(int privilege, void * start_routine, void * arg){
	tcbADT newTCB =	malloc(sizeof(tcbADT));
	newTCB->tid = tid++;
	newTCB->pid = getCurrentProcess();
	newTCB->context = createThreadContext(getContextOfSibling(newTCB->pid), start_routine, arg); //si marti devuelve null explota todo
	newTCB->state = NEW;
	addThread(newTCB);
	return newTCB;
}