#include <scheduler.h>

int addThread(tcbADT t){
  return enqueue(t);
}

void dispatchNextProcess(){
  tcbADT next = dequeue();
  int isNew = (next->state == NEW);
  next->state = RUNNING;
  dispatch(next->stack, isNew?executableMemoryAdress:0x0);
}

void initScheduler(){
   queue = malloc(sizeof(queueCDT));
   if (!queue)
           return NULL;
   queue->back = NULL;
   queue->front = NULL;
}

static int isEmpty(){
   if (!queue)
           return 1;
   if (!queue->front)
           return 1;
   else
           return 0;
}

static int enqueue(tcbADT elem){
   qnode *new = malloc(sizeof(qnode));
   if (!new)
           return 0;
   if (!q || !elem){
           free(new);
           return 0;
   }
   new->elem = elem;
   new->next = queue->back;
   new->prev = NULL;
   if (queue->back)
           queue->back->prev = new;
   queue->back = new;
   if (!queue->front)
           queue->front = new;
   return 1;
}

static tcbADT dequeue(){
   qnode *prev;
   tcbADT elem;
   if (isempty())
           return NULL;
   prev = queue->front->prev;
   elem = queue->front->elem;
   free(queue->front);
   queue->front = prev;
   return elem;
}
