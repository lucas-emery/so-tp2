#include <stdint.h>
#include <stdio.h>
#include <process.h>

int main(int argc, char *argv[]) {
  if(argc != 2)
    return 1;
  char buffer[100];
  getVar(argv[1], buffer);
  printf("%s\n", buffer);
  return 0;
}
