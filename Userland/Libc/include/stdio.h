#ifndef STDIO_H
#define STDIO_H

void write(int fd, char* buffer, int size);
void read(int fd, char* buffer, int size);
void putchar(char c);
char getchar();
int printf(const char* format, ...);
int scanf(const char* format, ...);
int sscanf(const char* format, const char * str, ...);
int parseInt(char* string, int * value);
int readLine(char * buffer, int maxSize);

#endif
