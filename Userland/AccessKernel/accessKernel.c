#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	printf("Trying to access to address 0x0...\n");
	int * kernel = 0x0;
	printf("%x\n", *kernel);
	return 0;
}
