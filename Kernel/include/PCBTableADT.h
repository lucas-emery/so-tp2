#ifndef PCBTABLEADT_H
#define PCBTABLEADT_H

#define RUNNING 0
#define BLOCKED 1
#define READY 2
#define TRUE 1
#define FALSE 0
#define NULL 0
#define MAX_CHILDREN 10
#define MAX_SIZE 100

typedef struct pcbCDT;
typedef struct pcbCDT* pcbADT;

static int idCount = 0;
static pcbADT pcbTable[MAX_SIZE];
static int tableSize = 0;

int addPCB(int privilege);
void removePCB(int id);
void changeState(int id, int state);
int addChild(int fatherId);
int createChild(pcbADT father);
void processesInfo(char* buffer);
char* makeString(pcbADT process);
 
#endif