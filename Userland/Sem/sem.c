#include <stdint.h>
#include <stdio.h>
#include <semaphore.h>

int semId;

int main(int argc, char *argv[]) {
  semId = sem_open("topkek");
  sem_post(semId);
  return 0;
}
