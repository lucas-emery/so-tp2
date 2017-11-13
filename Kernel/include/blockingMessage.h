#ifndef _BLOCKING_MESSAGE_H_
#define _BLOCKING_MESSAGE_H_

#include <lib.h>
#include <MMU.h>
#include <scheduler.h>
#include <lib.h>

#define MAX_SIZE_BUFFER 10 //it's not the actual size of the buffer but the amount of times write() was executed

typedef struct messageCDT* messageADT;

int createMessage(char* name, int messageSize);
int openMessage(char* name);
void readMessage(int id, char* buffer);
void writeMessage(int id, char* content);
int closeMessage(int id);

#endif