#ifndef STDIO_H
#define STDIO_H

#define TRUE 1
#define FALSE 0

void write(int fd, char* buffer, int size);
void read(int fd, char* buffer, int size);
void putchar(char c);
char getchar();
int printf(char* format, ...);
int scanf(char* format, ...);
int sscanf(char* format, char * str, ...);
int parseInt(char* string, int * value);
int readLine(char * buffer, int maxSize);
void exit(int value);

#endif
