#ifndef _IPC_H_
#define _IPC_H_

#include <lib.h>
#include <MMU.h>

typedef struct sem_tCDT{
	int value;
	char* name;
	int id;
};

typedef struct sem_tCDT* sem_t;

static int semCount = 0;
static int id = 0;

int semOpen(char* name, int value);
void semClose(int id);
void semPost(int id);
void semWait(int id);
int exists(char* name);

#endif