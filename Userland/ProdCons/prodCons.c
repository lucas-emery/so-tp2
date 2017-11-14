#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <message.h>
#include <strings.h>

#define MAX 100

typedef struct arguments{
	int messageID;
	char finishThread;
}arguments;

typedef struct arguments* argumentsPointer;

argumentsPointer producers[MAX];
argumentsPointer consumers[MAX];

static int producerCount;
static int consumerCount;

static char createProducerLetter;
static char removeProducerLetter;
static char createConsumerLetter;
static char removeConsumerLetter;

void producer(argumentsPointer arg);
void consumer(argumentsPointer arg);
void createProducer(int);
void createConsumer(int);
void removeProducer();
void removeConsumer();

int main(int argc, char *argv[]) {
	if(argc <= 4){
		printf("Missing arguments. Check 'man prodcons' for further explanation.\n");
		return 1;
	}
	uint64_t messageID = (uint64_t) message_init("Diego", 5);
	argumentsPointer argConsumer = malloc(sizeof(arguments));
	consumerCount = 0;
	producerCount = 0;
	createProducerLetter = *argv[1];
	createConsumerLetter = *argv[2];
	removeProducerLetter = *argv[3];
	removeConsumerLetter = *argv[4];
	pthread_create((function)createProducer, (void*) messageID);
	pthread_create((function)removeProducer, (void*)0);
	pthread_create((function)createConsumer, (void*) messageID);
	pthread_create((function)removeConsumer, (void*)0);
	while(1);
	return 0;
}

void createProducer(int messageID){
	while(1){
		keyBlock(createProducerLetter);
		argumentsPointer argProducer = malloc(sizeof(arguments));
		argProducer->messageID = messageID;
		argProducer->finishThread = FALSE;
		producers[producerCount++] = argProducer;
		pthread_create((function)producer, (void*) argProducer);
	}
	pthread_exit();
}

void createConsumer(int messageID){
	while(1){
		keyBlock(createConsumerLetter);
		argumentsPointer argConsumer = malloc(sizeof(arguments));
		argConsumer->messageID = messageID;
		argConsumer->finishThread = FALSE;
		consumers[consumerCount++] = argConsumer;
		pthread_create((function)consumer, (void*) argConsumer);
	}
	pthread_exit();
}

void removeProducer(){
	while(1){
		keyBlock(removeProducerLetter);
		if(producerCount != 1){
			(producers[producerCount-1])->finishThread = TRUE;
			producerCount--;
		}
	}
	pthread_exit();
}

void removeConsumer(){
	while(1){
		keyBlock(removeConsumerLetter);
		if(consumerCount != 1){
			(consumers[consumerCount-1])->finishThread = TRUE;
			consumerCount--;
		}
	}
	pthread_exit();
}

void producer(argumentsPointer arg){
	int timeout = 10000000;
  	while(!arg->finishThread){
    	if(timeout == 0){
      		message_write(arg->messageID, "capo ");
			printf("Produce: capo\n");
      		timeout=10000000;
    	}
    	timeout--;
  	}
	pthread_exit();
}

void consumer(argumentsPointer arg){
	int timeout = 10000000;
  	while(!arg->finishThread){
    	if(timeout == 0){
      		char* buffer = malloc(100);
			message_read(arg->messageID, buffer);
			printf("Consume: %s\n", buffer);
      		timeout=10000000;
    	}
    	timeout--;
  	}
	pthread_exit();
}