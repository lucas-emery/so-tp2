#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <pcb.h>

extern char* moduleNames[];
extern pcbADT* pcbTable;

int createProcess(int moduleIndex, int argc, char * argv[]);

#endif
