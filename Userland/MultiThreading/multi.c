#include <stdint.h>
#include <stdio.h>
#include <semaphore.h>
#include <process.h>

int semId;

int doIt(int num){
  keyBlock('a');
  printf("%d", num);
  pthread_exit();
  return 0;
}


int main(int argc, char *argv[]) {
  //semId = sem_open("topkek");
  //sem_set(semId,0);
  int id = pthread_create((function) doIt,(void *) 0);
  //sem_wait(semId);
  //pthread_create((function) doIt,(void *) 1);
  //while(1);
  wait(id);
  printf("me mori\n");
  return 0;
}
