#include <message.h>
#include <stdio.h>

int message_init(char* name, int messageSize){
	return int80(14, (uint64_t) name, (uint64_t) messageSize, 0);
}

int message_open(char* name){
	return int80(20, (uint64_t) name, 0, 0);
}

int message_close(int id){
	return int80(21, (uint64_t) id, 0, 0);
}

int message_write(int id, char* content){
	return int80(22, (uint64_t) id, (uint64_t) content, 0);
}

int message_read(int id, char* buffer){
	return int80(23, (uint64_t) id, (uint64_t) buffer, 0);
}
