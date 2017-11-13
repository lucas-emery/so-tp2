#include <stdint.h>
#include <stdio.h>
#include <process.h>


int main(int argc, char *argv[]) {
  int x;
  parseInt(argv[1], &x);
	kill(x);
	return 0;
}
