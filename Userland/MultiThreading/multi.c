#include <stdint.h>
#include <stdio.h>
#include <semaphore.h>
#include <process.h>

int semId;

int doIt(int num){
  int timeout = 10000000;
  while(1){
    if(timeout == 0){
      printf("%d",num);
      timeout=10000000;
    }
    timeout--;
  }
  pthread_exit();
  return 0;
}


int main(int argc, char *argv[]) {
  semId = sem_open("topkek");
  sem_set(semId,0);
  pthread_create((function) doIt,(void *) 1);
  sem_wait(semId);
  pthread_create((function) doIt,(void *) 1);
  while(1);
  return 0;
}
