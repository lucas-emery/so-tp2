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
	uint64_t messageID = (uint64_t) message_init("Diego", 4);
	pthread_create((function) producer, (void*) messageID);
	pthread_create((function) consumer, (void*) messageID);
	while(1);
}

int producer(int messageID){
	/*while(1){
		//keyBlock('p');
		
		message_write(messageID, "capo");
		printf("Produce: capo\n");
	}*/
	int timeout = 10000000000000;
  	while(1){
    	if(timeout == 0){
      		message_write(messageID, "capo");
			printf("Produce: capo\n");
      		timeout=10000000;
    	}
    	timeout--;
  	}
	pthread_exit();
}

int consumer(int messageID){
	/*while(1){
		//keyBlock('c');
		char* buffer = malloc(100);
		message_read(messageID, buffer);
		printf("Consume: %s\n", buffer);
	}*/
	int timeout = 10000000000000;
  	while(1){
    	if(timeout == 0){
      		char* buffer = malloc(100);
			message_read(messageID, buffer);
			printf("Consume: %s\n", buffer);
      		timeout=10000000;
    	}
    	timeout--;
  	}
	pthread_exit();
}
