#ifndef PCB_H
#define PCB_H

#include <lib.h>
#include <MMU.h>
#include <thread.h>
#include <context.h>

typedef struct pcbCDT* pcbADT;

/*
*Initializes the PCB table by giving it memory
*/
void setupPCB();

/*
*Creates a new PCB entry in the PCB table with the privilidge in parameter
*Returns the new processes pid
*/
int addPCB(char* name, int privilege);

void addTCB(tcbADT thread);
context_t* getContextOfSibling(int id);
tcbADT getThreads(int id, int* count);

/*
*Removes the PCB entry correspoding to the processes id in parameter
*Returns 0 if the process was found successfully, -1 if not
*/
int removePCB(int id);

/*
*Changes the state of the process witch has id in parameter to the new state in parameter
*Returns 0 if the process was found successfully, -1 if not
*/
int changeState(int id, int state);

/*
*Leaves in buffer the string containing the processes information
*/
void processesInfo(char* buffer);

/*
*Searches for the state of the process corresponding to the id in parameter
*Returns the state of the process if it was found successfully, -1 if not
*/
int getState(int id);

#endif
