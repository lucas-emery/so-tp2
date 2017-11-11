#include <message.h>

void message_open(char* name, int messageSize){
	int80(20, name, messageSize, 0);
}

void message_close(char* name){
	int80(21, name, 0, 0);
}

void message_write(char* name, char* content){
	int80(22, name, content, 0);
}

void message_read(char* name, char* buffer){
	int80(23, name, buffer, 0);
}