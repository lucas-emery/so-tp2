#include <scheduler.h>
#include <stdint.h>



static queueADT RRqueue;
static queueADT * semQueues, * rMsgQueues, * wMsgQueues;
static qnode * current = NULL;

uint8_t initScheduler(){
  RRqueue = initQueue();
  return RRqueue != NULL;
}

uint8_t addThread(tcbADT t){
  if(t == NULL)
    return 0;
  t->state = NEW;
  qnode * new = (qnode*) malloc(sizeof(qnode));
  new->thread = t;
  if(current == NULL){
    current = new;
    return SUCCESS;
  }
  return enqueue(RRqueue,new);
}

void killThread(){
  current->thread->state = DEAD;
  sti();
  while(1);
}

int getCurrentProcess(){
  return current->thread->pid;
}

void schedule(){
  if(current != NULL && current->thread->state != BLOCKED && current->thread->state != DEAD){
     current->thread->state = READY;
     enqueue(RRqueue, current);
  }

  current = dequeue(RRqueue);

  if(current->thread->state == DEAD){
    schedule();
    return;
  }

  setContext(current->thread->context);
  current->thread->state = RUNNING;
}

static uint8_t open(int i, queueADT ** array){
  *array = (queueADT *) realloc(*array, (i+1)*sizeof(queueCDT));
  if(*array == NULL)
    return FAIL;
  *array[i] = initQueue();
  if(*array[i] == NULL)
    return FAIL;
  return SUCCESS;
}

static void block(int i, queueADT * queueArray, int blocking){
  if(queueArray == NULL)
    return;
  current->thread->state = BLOCKED;
  saveContext();
  enqueue(queueArray[i],current);
}

static uint8_t unblock(int i, queueADT * queueArray){
  if(queueArray[i] == NULL)
    return FAIL;
  qnode * node = dequeue(queueArray[i]);
  if(node == NULL)
    return FAIL;
  node->thread->state = READY;
  return enqueue(RRqueue,node);
}

uint8_t initMsg(int msgId){
  return open(msgId,&rMsgQueues) && open(msgId, &wMsgQueues);
}

void destroyMsg(int msgId){
  free(rMsgQueues[msgId]);
  rMsgQueues[msgId] = NULL;
  free(wMsgQueues[msgId]);
  wMsgQueues[msgId] = NULL;
}

void msgBlock(int msgId,int type, int blocking){
  block(msgId, type==READ?rMsgQueues:wMsgQueues, blocking);
}

uint8_t msgUnblock(int msgId, int type){
  return unblock(msgId, type==READ?rMsgQueues:wMsgQueues);
}

uint8_t initSem(int semId){
  return open(semId,&semQueues);
}

void destroySem(int semId){
  free(semQueues[semId]);
  semQueues[semId] = NULL;
}

void semBlock(int semId,int blocking){
  block(semId, semQueues, blocking);
}

uint8_t semUnblock(int semId){
  return unblock(semId,semQueues);
}

void printQueue(queueADT q){
  qnode * current = q->back;
  while(current != NULL){
    printHex(current->thread->tid);
    current = current->prev;
  }
}


static queueADT initQueue(){
  queueADT ret = (queueADT) malloc(sizeof(queueCDT));
  if(ret == NULL)
    return NULL;
  ret->back = NULL;
  ret->front = NULL;
  return ret;
}

static int isEmpty(queueADT q){
  if(q == NULL)
    return 1;
  return q->front == NULL;
}

static uint8_t enqueue(queueADT q, qnode* node){
  if(q == NULL || node == NULL)
    return FAIL;
  node->next = q->back;
  node->prev = NULL;

  if(q->back)
    q->back->prev = node;

  q->back = node;

  if(!q->front)
    q->front = node;

  return SUCCESS;
}

static qnode* dequeue(queueADT q){
  if(isEmpty(q))
    return NULL;
  qnode * aux = q->front;
  aux->next = NULL;
  q->front = aux->prev;
  aux->prev = NULL;
  if(q->front != NULL)
    q->front->next = NULL;
  else
    q->back = NULL;
  return aux;
}
