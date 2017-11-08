#ifndef PCBTABLEADT_H
#define PCBTABLEADT_H

#include <lib.h>
#include <MMU.h>
#include <thread.h>

#define RUNNING 0
#define BLOCKED 1
#define READY 2
#define NEW 3
#define MAX_CHILDREN 10

typedef struct pcbCDT* pcbADT;

/*
*Creates a new PCB entry in the PCB table with the privilidge in parameter 
*Returns the new processes pid
*/
int addPCB(int privilege);

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
*Adds a new child to the process with id in parameter
*Returns child id if the process was found successfully, -1 if not
*/
int addChild(int fatherId);

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