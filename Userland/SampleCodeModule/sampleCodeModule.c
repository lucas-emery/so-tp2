/* sampleCodeModule.c */
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <message.h>
#include <strings.h>

//char * v = (char*)0xB8000 + 79 * 2;

int producer(int messageID);
int consumer(int messageID);

int main(int argc, char *argv[]) {
	//while(1){

	//}
	int messageID = message_init("Diego", 5);
	//printf("%d\n", messageID);
	pthread_create(producer, messageID);
	pthread_create(consumer, messageID);
	while(1);
}

int producer(int messageID){
	while(1){
		message_write(messageID, "capo");
		printf("Produce: capo\n");
	}
}

int consumer(int messageID){
	while(1){
		char* buffer = malloc(100);
		message_read(messageID, buffer);
		printf("Consume: %s\n", buffer);
	}
}
