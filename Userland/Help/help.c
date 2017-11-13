#include <stdint.h>
#include <stdio.h>
#include <process.h>

static const char * man =
"time: Prints out the current time\n\
date: Prints out the current date\n\
ps: Prints out processes information\n\
clear: Wipes out the screen\n\
hello: Prints arguments\n\
sampleCodeModule: A sample module\n\
philosophers: Solution to philosophers problem\n\
prodcons: Solution to producer-consumer problem";

int main(int argc, char *argv[]) {
	printf("%s\n",man);
	return 0;
}
