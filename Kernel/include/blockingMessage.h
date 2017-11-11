#ifndef _BLOCKING_MESSAGE_H_
#define _BLOCKING_MESSAGE_H_

#include <lib.h>
#include <MMU.h>
#include <scheduler.h>

#define BYTES 4
#define MAX_SIZE_BUFFER 100

typedef struct messageCDT* messageADT;

int initMessage(char* name);
void readMessage(char* name, char* buffer);
void writeMessage(char* name, char* content);
int closeMessage(char* name);

#endif