#ifndef _BLOCKING_MESSAGE_H_
#define _BLOCKING_MESSAGE_H_

#include <lib.h>
#include <MMU.h>
#include <scheduler.h>
#include <lib.h>

#define MESSAGE_OPERATIONS 5

#define MAX_SIZE_BUFFER 10 //it's not the actual size of the buffer but the amount of times write() was executed

typedef struct messageCDT* messageADT;
typedef int (*messageOperation)(char*, int);

extern int testAndSet(uint64_t lock);

void msgString(char* buffer);
int createMessage(char* name, int messageSize);
int openMessage(char* name, int arg2);
int readMessage(char* buffer, int id);
int writeMessage(char* content, int id);
int closeMessage(char*arg1, int id);
int executeMessage(int operation, char* arg1, int arg2);
void setupMessages();

#endif
