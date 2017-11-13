#include <message.h>

int message_init(char* name, int messageSize){
	return int80(14, name, messageSize, 0);
}

int message_open(char* name){
	return int80(20, name, 0, 0);
}

int message_close(int id){
	return int80(21, id, 0, 0);
}

int message_write(int id, char* content){
	return int80(22, id, content, 0);
}

char* message_read(int id, char* buffer){
	int80(23, id, buffer, 0);
	return buffer;
}