#include <stdint.h>
#include <stdio.h>

int semId;

int main(int argc, char *argv[]) {
  semId = sem_open("topkek",-1);
  sem_post(semId);
  return 0;
}
