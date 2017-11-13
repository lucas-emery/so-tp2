#include <stdint.h>
#include <stdio.h>
#include <process.h>


int main(int argc, char *argv[]) {
  if(argc != 2)
    return 1;
  printf("%d\n", getVar(argv[1]));
  return 0;
}
