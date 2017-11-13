#include <stdint.h>
#include <stdio.h>
#include <process.h>

int main(int argc, char *argv[]) {
	char* buffer = malloc(100);
	ps(buffer);
	printf("%s\n", buffer);
	return 0;
}
