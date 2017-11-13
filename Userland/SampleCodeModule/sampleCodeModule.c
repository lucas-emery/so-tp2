/* sampleCodeModule.c */
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <message.h>
#include <strings.h>

//char * v = (char*)0xB8000 + 79 * 2;

int haceMensaje(int messageID);
int leeMensaje(int messageID);

int main(int argc, char *argv[]) {
	//while(1){
	
	//}
	int messageID = message_init("Diego", 5);
	//printf("%d\n", messageID);
	pthread_create(haceMensaje, messageID);
	pthread_create(leeMensaje, messageID);
	while(1);
}

int haceMensaje(int messageID){
	while(1){
		message_write(messageID, "capo");
		char* buffer = malloc(100);
		buffer = message_read(messageID, buffer);
		printf("%s\n", buffer);
	}
}

int leeMensaje(int messageID){
	while(1){
		char* buffer = malloc(100);
		message_read(messageID, buffer);
		if(buffer[0] != 0)
			printf("%s\n", buffer);
		else
			printf("No leyo\n");
	}
}