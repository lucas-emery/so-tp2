#include <blockingMessage.h>

typedef struct messageCDT{
	const char* name;
	int id;
	char* buffer;
	int contentCount;
	int messageSize;
}messageCDT;

static int id = 0;
static int messagesCount = 0;
static messageADT* messages;

void initMessages(){
	messages = malloc(sizeof(messageADT));
}

int createMessage(char* name, int messageSize){
	messageADT newMessage = malloc(sizeof(messageCDT));
	strcpy(newMessage->name, name);
	newMessage->id = id;
	newMessage->messageSize = messageSize;
	id++;
	newMessage->buffer = malloc(sizeof(char));
	newMessage->contentCount = 0;
	messagesCount++;
	messages = realloc(messages, messagesCount * sizeof(messageADT));
	messages[messagesCount-1] = newMessage;
	initMsg(newMessage->id); //creates queue
	return newMessage->id;
}

void readMessage(char* name, char* buffer){
	for (int i = 0; i < messagesCount; ++i){
		if(strcmp(messages[i]->name, name)){
			if(messages[i]->contentCount == 0)
				msgBlock(id, READ, FALSE);
			else{
				msgUnblock(messages[i]->id, WRITE);
				strcpy(buffer, messages[i]->buffer);
				messages[i]->contentCount = 0;
				messages[i]->buffer = realloc(messages[i]->buffer, sizeof(char));
			}
		}
	}
}

void writeMessage(char* name, char* content){
	for (int i = 0; i < messagesCount; ++i){
		if(strcmp(messages[i]->name, name)){
			if(messages[i]->contentCount == MAX_SIZE_BUFFER)
				msgBlock(messages[i]->id, WRITE, FALSE);
			else{
				msgUnblock(messages[i]->id, READ);
				messages[i]->contentCount++;
				messages[i]->buffer = realloc(messages[i]->buffer, messages[i]->contentCount * messages[i]->messageSize);
				strcat(messages[i]->buffer, content);
			}
		}
	}
}

int closeMessage(char* name){
	for (int i = 0; i < messagesCount; ++i){
		if(strcmp(messages[i]->name, name)){
			destroyMsg(messages[i]->id); //remove queue
			free(messages[i]);
			for(int j = i; j < messagesCount-1; j++)
				messages[j] = messages[j+1];
			messagesCount--;
			return SUCCESS;
		}
	}
	return FAIL;
}