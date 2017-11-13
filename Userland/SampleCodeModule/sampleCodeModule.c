/* sampleCodeModule.c */
#include <stdio.h>

char * v = (char*)0xB8000 + 79 * 2;

int main(int argc, char *argv[]) {
	int kek;
	scanf("%d", &kek);
	printf("Read: %d", kek);
	return 1;
}
