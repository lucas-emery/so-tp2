#ifndef _BLOCKING_MESSAGE_H_
#define _BLOCKING_MESSAGE_H_

#include <lib.h>
#include <MMU.h>

#define BYTES 4
#define MAX_SIZE_BUFFER 100

typedef struct messageCDT* messageADT;

int initMessage(char* name, char* content);
void readMessage(int id, char* buffer);
void writeMessage(int id, char* content);

#endif