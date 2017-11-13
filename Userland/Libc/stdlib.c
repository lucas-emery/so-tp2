#include <stdlib.h>

static unsigned long int next = 1;

void * malloc(unsigned int size) {
	void * address;
	int80(8,(uint64_t) &address, (uint64_t) size,0);
	return address;
}

void free(void *ptr) {

}

//Adapted from naiveterminal.c
int itoa(uint64_t value, char * buffer, int base) {
	char *p = buffer;
	char *p1, *p2;
	int digits = 0;

	if(value < 0 && base == 10) {
		value = -value;
		*p++ = '-';
		digits++;
	}

	//Calculate characters for each digit
	do
	{
		int remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = *buffer == '-' ? buffer + 1: buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

int isAlpha(char c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int isDigit(char c) {
	return (c >= '0' && c <= '9');
}

int rand(){
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}
