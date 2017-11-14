#include <stdio.h>
#include <process.h>

int main(){
	char* buffer = malloc(100);
	ts(buffer);
	printf("%s", buffer);
	return 0;
}