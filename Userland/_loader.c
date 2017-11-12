/* _loader.c */
#include <stdint.h>
#include <process.h>

extern char bss;
extern char endOfBinary;

int main(int argc, char *argv[]);

void * memset(void * destiny, int32_t c, uint64_t length);


int _start(int argc, char *argv[]) {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	exit(main(argc, argv));
	return  0;
}


void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
