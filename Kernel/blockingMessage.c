#include <blockingMessage.h>

typedef struct messageCDT{
	const char* name;
	int id;
	char* buffer;
	int contentCount;
}messageCDT;

static int id = 0;
static int messagesCount = 0;
static messageADT* messages;

void initMessages(){
	messages = malloc(sizeof(messageADT));
}

int createMessage(char* name, char* content){
	messageADT newMessage;
	newMessage->name = name;
	newMessage->id = id;
	id++;
	newMessage->buffer = malloc(sizeof(char));
	newMessage->buffer = content;
	newMessage->contentCount = 1;
	messagesCount++;
	messages = realloc(messages, messagesCount * sizeof(messageADT));
	messages[messagesCount-1] = newMessage;
	return newMessage->id;
}

void readMessage(int id, char* buffer){
	for (int i = 0; i < messagesCount; ++i){
		if(messages[i]->id == id){
			if(messages[i]->contentCount == 0)
				//msgBlock(id, READ, FALSE);
			//else{
				buffer = messages[i]->buffer;
				messages[i]->contentCount = 0;
				messages[i]->buffer = realloc(messages[i]->buffer, sizeof(char));
			//}
		}
	}
}

void writeMessage(int id, char* content){
	for (int i = 0; i < messagesCount; ++i){
		if(messages[i]->id == id){
			if(messages[i]->contentCount == MAX_SIZE_BUFFER)
				//msgBlock(id, WRITE, FALSE);
			//else{
				messages[i]->contentCount++;
				messages[i]->buffer = realloc(messages[i]->buffer, messages[i]->contentCount * BYTES);
				strcat(messages[i]->buffer, content);
			//}
		}
	}
}