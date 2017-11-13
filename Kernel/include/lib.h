#ifndef _LIB_H_
#define _LIB_H_

#include <stdint.h>
#include <stddef.h>

#define TRUE 1
#define FALSE 0

#define SUCCESS 0
#define FAIL 1

//messages
#define OPEN 0
#define CLOSE 1
#define INIT 2
#define READ 3
#define WRITE 4

//semaphores
#define OPEN 0
#define CLOSE 1
#define POST 2
#define WAIT 3
#define SET 4

//scheduler
#define MAX_QUEUES 10
#define READ 3
#define WRITE 4
#define SEM 5
#define STDIN 6
#define KEY 7
#define waitThread 8

//threads
#define RUNNING 0
#define BLOCKED 1
#define READY 2
#define NEW 3
#define DEAD 4

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
int strcmp(const char *str1, const char *str2);
char* strcat(char *dest, const char *src);
char* strcpy(char* dest, char* src);
size_t strlen(const char *str);

void intTT();
void sti();
void cli();
void dispatch(uint64_t,uint64_t);
char *cpuVendor(char *result);
void writePort(uint8_t port, uint8_t value);
int readPort(uint8_t port);

#endif
