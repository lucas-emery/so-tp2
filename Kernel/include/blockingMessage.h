#ifndef _BLOCKING_MESSAGE_H_
#define _BLOCKING_MESSAGE_H_

#include <lib.h>

#define BYTES 4
#define MAX_SIZE_BUFFER 100

typedef char[BYTES] content_t;
typedef struct messageCDT;
typedef struct messageCDT* messageADT;

int initMessage(char* name, content_t content);
void readMessage(int id, char* buffer);
void writeMessage(int id, content_t content);

#endif