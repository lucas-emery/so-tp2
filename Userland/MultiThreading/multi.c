#include <stdint.h>
#include <stdio.h>

int semId;

int doIt1(){
  sem_wait(semId);
  while(1){
    printf("%d\n",2);
  }
  return 0;
}

int doIt2(){
  while(1){
    sem_wait(semId);
    printf("%d\n",2);
    sem_post(semId);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  semId = sem_open("topkek", 0);
  pthread_create(doIt1,0);
  //pthread_create(doIt2,0);

  while(1);
  sem_close(semId);
  return 0;
}
