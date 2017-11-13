#include <stdint.h>
#include <stdio.h>
#include <semaphore.h>
#include <process.h>

int semId;

int doIt(int num){
  while(1){
    keyBlock('a');
    printf("%d", num);
  }
  pthread_exit();
  return 0;
}


int main(int argc, char *argv[]) {
  semId = sem_open("topkek");
  sem_set(semId,0);
  pthread_create((function) doIt,(void *) 0);
  sem_wait(semId);
  pthread_create((function) doIt,(void *) 1);
  while(1);
  return 0;
}
