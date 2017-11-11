#include <scheduler.h>
#include <stdint.h>



queueADT RRqueue;
queueADT * semQueues, * rMsgQueues, * wMsgQueues;
uint8_t semCount = 0;
qnode * current = NULL;

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

int getCurrentProcess(){
  return current->thread->pid;
}

void schedule(){
  if(current != NULL && current->thread->state != BLOCKED){
     current->thread->state = READY;
     enqueue(RRqueue, current);
  }
  printHex(current->thread->tid);
  current = dequeue(RRqueue);
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
  if(enqueue(queueArray[i],current) == FAIL)
    return;
  return; //yield.
}

static uint8_t unblock(int i, queueADT * queueArray){
  if(queueArray[i] == NULL)
    return FAIL;
  qnode * node = dequeue(queueArray[i]);
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
  node->next = NULL;
  node->prev = q->back;

  if(q->back)
    q->back->next = node;

  q->back = node;

  if(!q->front)
    q->front = node;

  return SUCCESS;
}

static qnode* dequeue(queueADT q){
  if(isEmpty(q))
    return NULL;
  qnode *next;
  next = q->front->next;
  qnode * aux = q->front;
  free(q->front);
  q->front = next;
  return aux;
}
