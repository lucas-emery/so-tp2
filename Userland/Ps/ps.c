#include <stdint.h>
#include <stdio.h>
#include <process.h>

int main(int argc, char *argv[]) {
	char* buffer = malloc(100);
	printf("%l\n", buffer);
	printf("%s\n", ps(buffer));
	return 0;
}
