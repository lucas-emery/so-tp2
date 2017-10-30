#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <strings.h>

#define BUFFER_SIZE 25*80+1000

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

void write(int fd, char* buffer, int size) {
	int80(1,fd,buffer,size);
}

void read(int fd, char* buffer, int size) {
	int80(0,fd,buffer,size);
}

void putchar(char c) {
	write(1, &c, 1);
}

char getchar() {
	char ch;
	do
		read(0, &ch, 1);
	while(ch == '\0');
	return ch;
}

int printf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int num, length;
	char number[12];
	char ch;
	char* string;

	while((ch = *(format++)) != '\0') {
		if(ch == '%') {
			switch(ch = *(format++)) {
				case 'd':
					num = va_arg(args, int);
					length = itoa(num, number, 10);
					write(1, number,length);
					break;
				case 'c':
					putchar((char)va_arg(args, int));
					break;
				case 's':
					string = (char*)va_arg(args,char*);
					write(1,string,strlen(string));
					break;
			}
		}
		else
			putchar(ch);
	}
	return 0;
}

int scanf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int read = 0;
	int index = 0;
	int length;
	char ch;
	char buffer[BUFFER_SIZE];
  char* character;

  length = readLine(buffer,BUFFER_SIZE);

	while((ch = *(format++)) != '\0') {
		if(ch == '%') {
			switch(ch = *(format++)) {
				case 'd':
          index += parseInt(&buffer[index], va_arg(args,int*));
					read++;
					break;
				case 'c':
        	character = va_arg(args, char*);
          *character = buffer[index++];
          read++;
          break;
				case 's':
          character = va_arg(args,char*);
          char aux;
          while( (aux = buffer[index++]) != '\0')
          	*(character++) = aux;
          *character = '\0';
          read++;
          break;
			}
		}
		if(ch != buffer[index++])
			return read;
  }
	return read;
}

int sscanf(const char* format, const char* str, ...) {
	va_list args;
	va_start( args, format );
	int read = 0;
	int index;
	char ch;
  char* character;

	while((ch = *(format++)) != '\0') {
		if(ch == '%') {
			switch(ch = *(format++)) {
				case 'd':
        	index = parseInt(str, va_arg(args,int*));
          str += index;
					read++;
					break;
				case 'c':
          character = va_arg(args, char*);
          *character = *(str++);
          read++;
          break;
				case 's':
          character = va_arg(args,char*);
          char aux;
          while( (aux = *(str++)) != '\0')
          	*(character++) = aux;
          *character = '\0';
          read++;
          break;
			}
		}
		if(ch != *(str++))
			return read;
  }
	return read;
}


int parseInt(char* string, int * value) {
  *value = 0;
	int sign= 1;
	int digits = 0;
	char c;

  if(*string == '-') {
		if (isDigit(*(string + 1))) {
			sign = -1;
			digits++;
			string++;
		}
		else
			return 0;
	}

  while (isDigit(c = *string)){
		*value = (*value)*10+(c - '0');
    digits++;
    string++;
  }

  *value *= sign;

  return digits;
}

int readLine(char * buffer, int maxSize) {
	int length = 0;
	char c;

	while((c = getchar()) != '\n' && length < maxSize) {
		*(buffer++) = c;
		length++;
	}

	*buffer = '\0';
	return length;
}
