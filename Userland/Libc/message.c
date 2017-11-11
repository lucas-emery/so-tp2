#include <message.h>

message_open(char* name){
	int80(20, name, 0, 0);
}

message_close(char* name){
	int80(21, name, 0, 0);
}

message_write(char* name, char* content){
	int80(22, name, content, 0);
}

message_read(char* name, char* buffer){
	int80(23, name, buffer, 0);
}