#ifndef _BLOCKING_MESSAGE_H_
#define _BLOCKING_MESSAGE_H_

#include <lib.h>
#include <MMU.h>
#include <scheduler.h>
#include <lib.h>

#define MAX_SIZE_BUFFER 50 //it's not the actual size of the buffer but the amount of times write() was executed

typedef struct messageCDT* messageADT;

int initMessage(char* name, int messageSize);
void readMessage(char* name, char* buffer);
void writeMessage(char* name, char* content);
int closeMessage(char* name);

#endif