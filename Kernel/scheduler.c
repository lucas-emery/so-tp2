#include <scheduler.h>

queueADT RRqueue;
queueADT * semQueues;
uint8_t semCount = 0;
tcbADT current;

uint8_t initScheduler(){
  RRqueue = initQueue();
  return RRqueue != NULL;
}

uint8_t addThread(tcbADT t){
  if(t == NULL)
    return 0;
  t->state = NEW;
  return enqueue(RRqueue, (void*) t);
}

int getCurrentProcess(){
  return current->pid;
}

void schedule(){
  current->state = READY;
  enqueue(RRqueue, (void*) current);
  current = (tcbADT) dequeue(RRqueue);
  setContext(current->context);
  current->state = RUNNING;
}

uint8_t semOpen(int semId){
  semQueues = realloc(semQueues,(semId+1)*sizeof(queueADT));
  if(!semQueues)
    return 0;
  semQueues[semId] = malloc(sizeof(queueCDT));
  if(!semQueues[semId])
    return 0;
  return 1;
}

void semClose(int semId){
  free(semQueues[semId]);
  semQueues[semId] = NULL:
}

uint8_t semBlock(int semId){
  if(semQueues[semId] == NULL)
    return FAIL;
  threadPackADT pack = malloc(sizeof(packCDT));
  if(pack == NULL)
    return FAIL;
  pack->threads = getThreads(getCurrentThread(),pack->count);
  if(!enqueue(semQueues[semId],pack))
    return FAIL;
  managePack(pack,BLOCK);
  return SUCCESS;
}

uint8_t semUnblock(int semId){
  if(semQueues[semId] == NULL)
    return FAIL;
  threadPackADT pack = (threadPackADT*) dequeue(semQueues[semId]);
  managePack(pack,UNBLOCK);
}

static void managePack(threadPackADT pack,uint8_t type){
  if(pack == NULL)
    return;

  for(int i = 0;i < pack->count; i++){
    if(type == BLOCK)
      remove(RRqueue, pack->threads[i]);
    else
      enqueue(RRqueue,pack->threads[i]);
    pack->threads[i]->state = (type==BLOCK)?BLOCKED:READY;
  }

}

static queueADT initQueue(){
  queueADT ret = malloc(sizeof(queueCDT));
  if(ret == NULL)
    return NULL;
  ret->back = NULL;
  ret->front = NULL;
  return ret;
}

static int isEmpty(queueADT q){
  if(q == NULL)
    return 1;
  return q->front != NULL;
}

static uint8_t enqueue(queueADT q,void* elem){
  if(q == NULL || elem == NULL)
    return FAIL;
  qnode *new = malloc(sizeof(qnode));
  if(new == NULL)
    return FAIL;
  new->elem = elem;
  new->next = q->back;
  new->prev = NULL;

  if(q->back)
    q->back->prev = new;

  q->back = new;

  if(!q->front)
    q->front = new;

  return SUCCESS;
}

static void remove(queueADT q, void* elem){
  removeR(q,elem,q->front);
}

static void removeR(queueADT q, void* elem, qnode current){
  if(current == NULL)
    return;

  if(current->elem == elem){
    qnode aux = current->next;
    if(aux != NULL)
      aux->prev = current->prev;
    if(prev != NULL)
      prev->next = aux;
    free(current);
    return;
  }

  removeR(q,elem,current->next);
}

static void* dequeue(queueADT q){
  if(isEmpty(q))
    return NULL;
  qnode *prev;
  void* elem;
  prev = q->front->prev;
  elem = q->front->elem;
  free(q->front);
  q->front = prev;
  return elem;
}