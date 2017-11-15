#include <stdint.h>
#include <stdio.h>
#include <process.h>

static const char * man =
"time: Prints out the current time\n\
date: Prints out the current date\n\
man: Provides information about a module.\n\
ls: Prints available modules.\n\
ps: Prints out processes information\n\
ts: Prints out threads information\n\
clear: Wipes out the screen\n\
hello: Prints arguments\n\
accessKernel: Attempts to read data from the kernel.\n\
echo: Prints system variables.\n\
kill: Kills the specified process.\n\
multi: Demonstration of multithreading and semaphore. (used with sem)\n\
sem: Performs a sem_post on the 'topkek' semaphore.\n\
philosophers: Solution to philosophers problem\n\
prodcons: Solution to producer-consumer problem";

int main(int argc, char *argv[]) {
	printf("%s\n",man);
	return 0;
}
