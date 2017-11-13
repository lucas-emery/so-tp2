#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int * kernel = 0x0;
  printf("%x\n", *kernel);
  return 0;
}
