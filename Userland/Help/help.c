#include <stdint.h>
#include <stdio.h>

static const char * man =
"time: Prints out the current time\n\
date: Prints out the current date\n\
clear: Wipes out the screen\n\
hello: Prints arguments\n\
sampleCodeModule: A sample module";

int main(int argc, char *argv[]) {
  printf("%s\n", man);
  return 0;
}
