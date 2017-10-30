#include <stdint.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
  printf("%d:%d:%d\n", getHour(), getMinutes(), getSeconds());
  return 0;
}
