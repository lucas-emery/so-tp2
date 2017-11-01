#include <PCBTableADT.h>
#include <lib.h>

typedef struct pcbCDT
{
	int pid;
	int privilege;
	int state;
	int* children;
	uint64_t* programCounter;
	uint64_t* stack;
	struct PCB* next;
};

void addPCB(int privilege)
{
	pcbADT new;//reservar memoria
	new->pid = idCount;
	idCount++;
	new->state = READY;
	new->privilege = privilege;
	new->next = NULL;
	//new->programCounter=dir;
	//new->stack=dir;
	if(last == NULL)
		last = new;
	else
		last->next = new;
}

void removePCB(int id){
	pcbTable = remove(pcbTable, id);
}

pcbADT remove(pcbADT current, int id){
	pcbADT aux;
	if(current == NULL)
		return current;
	if(current->pid == id)
	{
		aux = current->next;
		//free current
		return aux;
	}
	current->next = remove(current->next, id);
	return current;
}

void changeState(int id, int state)
{
	pcbADT current = pcbTable;
	int found = FALSE;
	while(current != NULL || found){
		if(current->pid == id)
		{
			current->state = state;
			found = TRUE;
		}
		current = current->next;
	}
}



