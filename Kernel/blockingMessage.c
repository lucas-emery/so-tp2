#include <blockingMessage.h>

typedef struct messageCDT{
	const char* name;
	const int id;
	content_t* buffer;
	int contentCount;
};

static int id = 0;
static int messagesCount = 0;
static messageADT messages;

void initMessages(){
	messages = malloc(sizeof(messageADT));
}

int createMessage(char* name, content_t content){
	messageCDT newMessage;
	newMessage->name = name;
	newMessage->id = id++;
	newMessage->buffer = malloc(sizeof(*content_t));
	newMessage->buffer[0] = content;
	newMessage->contentCount = 1;
	messageCount++;
	messages = realloc(messages, messagesCount * sizeof(messageADT));
	messages[messagesCount-1] = newMessage;
	return newMessage->id;
}

void readMessage(int id, char* buffer){
	for (int i = 0; i < messagesCount; ++i){
		if(messages[i]->id == id){
			if(messages[i]->contentCount == 0)
				msgBlock(id, READ, FALSE);
			else{
				buffer = messages[i]->buffer;
				messages[i]->contentCount = 0;
				messages[i]->buffer = realloc(buffer, sizeof(*content_t));
			}
		}
	}
}

void writeMessage(int id, content_t content){
	for (int i = 0; i < messagesCount; ++i){
		if(messages[i]->id == id){
			if(messages[i]->contentCount == SIZE_BUFFER)
				msgBlock(id, WRITE, FALSE);
			else{
				messages[i]->contentCount++;
				messages[i]->buffer = realloc(buffer, contentCount * sizeof(*content_t));
				messages[i]->buffer[messages[i]->contentCount-1] = content;
			}
		}
	}
}