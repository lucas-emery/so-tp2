#include <stdint.h>
#include <stdio.h>
#include <process.h>


int main(int argc, char *argv[]) {
	if(argc < 2){
		printf("Missing arguments\n");
		return 1;
	}
  int x;
  parseInt(argv[1], &x);
	kill(x);
	return 0;
}
