#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <process.h>

#define BUFFERSIZE 1024

int main() {
  char buffer[BUFFERSIZE];
  while(1) {
    printf("$> ");
    scanf("%s", buffer);
    exec(buffer);
  }

  return 0;
}
