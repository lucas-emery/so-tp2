#include <scheduler.h>
#include <stdint.h>
#define IDLE 0
#define KEYS 'z' - 'a'

static queueADT RRqueue, stdinQueue;
static queueADT * semQueues, * rMsgQueues, * wMsgQueues, *keyQueues;
static qnode * idle, * current = NULL;
int keys[KEYS] = {0};
extern int moduleCount;


uint8_t initScheduler(){
  RRqueue = initQueue();
  stdinQueue = initQueue();
  createProcess(moduleCount - 1, 0, 0);
  idle = current;
  return (RRqueue == NULL) + (stdinQueue == NULL);
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
  intTT();
}

int getCurrentProcess(){
  return current->thread->pid;
}

void schedule(){
  if(current != NULL && current->thread->state != BLOCKED && current->thread->state != DEAD && current->thread->tid != IDLE){
     current->thread->state = READY;
     enqueue(RRqueue, current);
  }

  current = dequeue(RRqueue);
  //printHex(current->thread->tid);
  if(current == NULL){
    current = idle;
  }

  if(current->thread->state == DEAD){
    freeThreadContext(current->thread->context);
    schedule();
    return;
  }

  setContext(current->thread->context);
  current->thread->state = RUNNING;
}

uint8_t open(int i, queueADT ** arrayRef){
  *arrayRef = (queueADT *) realloc(*arrayRef, (i+1)*sizeof(queueCDT));
  queueADT * array = *arrayRef;

  if(array == NULL)
    return FAIL;
  array[i] = initQueue();
  if(array[i] == NULL)
    return FAIL;
  return SUCCESS;
}

queueADT getQueue(int i, int type){
  switch(type){
    case STDIN:
      return stdinQueue;
    case SEM:
      return semQueues[i];
    case READ:
      return rMsgQueues[i];
    case WRITE:
      return wMsgQueues[i];
    case KEY:{
      if(i < 'a' || i > 'z' || !keys[getKeyId()])
        return NULL;
      return keyQueues[getKeyId(i)];
    }
  }
  return NULL;
}

void block(int i, int type){
  current->thread->state = BLOCKED;
  if(type == KEY){ print("blocking: "); printDec(current->thread->tid); print(" | "); printDec(type); print(" | "); printHex(getQueue(i,type));print(" | ");}
  enqueue(getQueue(i,type),current);
	intTT();
}

uint8_t unblock(int i, int type){
  qnode * node = dequeue(getQueue(i,type));
  if(node == NULL)
    return FAIL;
  if(node->thread->state == DEAD){
    return unblock(i, type);
  }
  node->thread->state = READY;
  return enqueue(RRqueue,node);
}

int getKeyId(int key){
  return key - 'a';
}

uint8_t initKey(int key){
  if(key < 'a' || key > 'z')
    return FAIL;
  keys[getKeyId(key)] = 1;
  return !open(getKeyId(key),&keyQueues);
}

uint8_t initMsg(int msgId){
  return !open(msgId,&rMsgQueues) && !open(msgId, &wMsgQueues);
}

void destroyMsg(int msgId){
  free(rMsgQueues[msgId]);
  rMsgQueues[msgId] = NULL;
  free(wMsgQueues[msgId]);
  wMsgQueues[msgId] = NULL;
}

uint8_t initSem(int semId){
  return !open(semId,&semQueues);
}

void destroySem(int semId){
  free(semQueues[semId]);
  semQueues[semId] = NULL;
}

queueADT initQueue(){
  queueADT ret = (queueADT) malloc(sizeof(queueCDT));
  if(ret == NULL)
    return NULL;
  ret->back = NULL;
  ret->front = NULL;
  return ret;
}

int isEmpty(queueADT q){
  if(q == NULL)
    return 1;
  return q->front == NULL;
}

uint8_t enqueue(queueADT q, qnode* node){
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

qnode* dequeue(queueADT q){
  if(isEmpty(q)){
    return NULL;
  }
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
