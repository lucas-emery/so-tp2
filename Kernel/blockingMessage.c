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
static messageOperation messageOperations[MESSAGE_OPERATIONS];
static int lock = 0;


void msgString(char* buffer){
	strcat(buffer, "Messages: ");
	for(int i = 0; i < messagesCount ; i++){
		strcat(buffer,messages[i]->name);
		if(i != messagesCount -1)
			strcat(buffer, ", ");
	}
}

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

int openMessage(char* name, int arg2){
	for (int i = 0; i < messagesCount; ++i){
	for (int i = 0; i < messagesCount; i++){
		if(strcmp(messages[i]->name, name) == 0)
			return messages[i]->id;
	}
	return -1;
}

int readMessage(char* buffer, int id){
	printHex(buffer);
	for (int i = 0; i < messagesCount; ++i){
	for (int i = 0; i < messagesCount; i++){
		if(messages[i]->id == id){
			if(messages[i]->contentCount == 0)
				block(id, READ);
			else{
				strcpy(buffer, messages[i]->buffer);
				messages[i]->contentCount = 0;
				messages[i]->buffer = realloc(messages[i]->buffer, sizeof(char));
				unblock(messages[i]->id, WRITE);
			}
			return SUCCESS;
		}
	}
	return FAIL;
}

int writeMessage(char* content, int id){
	for (int i = 0; i < messagesCount; ++i){
	for (int i = 0; i < messagesCount; i++){
		if(messages[i]->id == id){
			if(messages[i]->contentCount == MAX_SIZE_BUFFER)
				block(messages[i]->id, WRITE);
			else{
				messages[i]->contentCount++;
				messages[i]->buffer = realloc(messages[i]->buffer, messages[i]->contentCount * messages[i]->messageSize);
				strcat(messages[i]->buffer, content);
				unblock(messages[i]->id, READ);
			}
			return SUCCESS;
		}
	}
	return FAIL;
}

int closeMessage(char*arg1, int id){
	for (int i = 0; i < messagesCount; ++i){
	for (int i = 0; i < messagesCount; i++){
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

int executeMessage(int operation, char* arg1, int arg2){
	if(operation < 0 || operation > MESSAGE_OPERATIONS)
		return -1;
	int ret;
	//testAndSet(&lock);
	ret = (messageOperations[operation])(arg1, arg2);
	lock = FALSE;
	return ret;
}

void setupMessages(){
	messageOperations[OPEN] = &openMessage;
	messageOperations[CLOSE] = &closeMessage;
	messageOperations[INIT] = &createMessage;
	messageOperations[READ] = &readMessage;
	messageOperations[WRITE] = &writeMessage;
}
