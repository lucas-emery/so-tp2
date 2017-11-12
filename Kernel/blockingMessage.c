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

int openMessage(char* name){
	for (int i = 0; i < messagesCount; ++i){
		if(strcmp(messages[i]->name, name) == 0)
			return messages[i]->id;
	}
	return -1;
}

void readMessage(int id, char* buffer){
	for (int i = 0; i < messagesCount; ++i){
		if(messages[i]->id == id){
			if(messages[i]->contentCount == 0)
				block(id, READ);
			else{
				unblock(messages[i]->id, WRITE);
				strcpy(buffer, messages[i]->buffer);
				messages[i]->contentCount = 0;
				messages[i]->buffer = realloc(messages[i]->buffer, sizeof(char));
			}
		}
	}
}

void writeMessage(int id, char* content){
	for (int i = 0; i < messagesCount; ++i){
		if(messages[i]->id == id){
			if(messages[i]->contentCount == MAX_SIZE_BUFFER)
				block(messages[i]->id, WRITE);
			else{
				unblock(messages[i]->id, READ);
				messages[i]->contentCount++;
				messages[i]->buffer = realloc(messages[i]->buffer, messages[i]->contentCount * messages[i]->messageSize);
				strcat(messages[i]->buffer, content);
			}
		}
	}
}

int closeMessage(int id){
	for (int i = 0; i < messagesCount; ++i){
		if(messages[i]->id == id){
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
