#include <scheduler.h>

static queueADT RRqueue, stdinQueue, semQueues[MAX_QUEUES], rMsgQueues[MAX_QUEUES], wMsgQueues[MAX_QUEUES], keyQueues[KEYS];
static wnode waitMap[MAX_THREADS];
static int waitCount;
static qnode * idle, * current = NULL;
extern uint32_t moduleCount;

void wait(int id){
  int firstFree = waitCount;
  for(int i = 0; i < waitCount; i++){
    if(waitMap[i].id == 0)
      firstFree = i;
    if(waitMap[i].id == id){
      threadCount++;
      waitMap[i].threads = realloc(waitMap[i].threads,threadCount * sizeof(*qnode));
      waitMap[i].threads[threadCount-1] = current;
      current->thread->state = BLOCKED;
      intTT();
      return;
    }
  }
  if(firstFree == waitCount && waitCount + 1 < MAX_THREADS){
    waitMap[firstFree].id = id;
    waitMap[firstFree].threads = NULL;
    waitMap[firstFree].threadCount = 0;
    waitCount++;
    intTT();
  }
}

/*
 *  Create an empty queue.
 *  Returns a queueADT on success, otherwise #FAIL.
 */
static queueADT initQueue();

/*
 * Check if queue is empty.
 * Returns #TRUE if the queue is empty, and otherwise #FALSE.
 */
static int isEmpty(queueADT q);

/*
 * Add @node to @q.
 * Returns #SUCCESS on success, and otherwise #FAIL.
 */
static uint8_t enqueue(queueADT q, qnode* node);

/*
 *  Remove last element from queue, and return it.
 *  Returns a qnode if @queueADT is not empty, and otherwise #NULL.
 */
static qnode* dequeue(queueADT q);

static queueADT getQueue(int index, int type);

static int getKeyId(int key);


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
  for(int i = 0; i < waitCount; i++){
    if(waitMap[i].id == current->thread->tid){
      for (int j = 0; j < waitMap[i].threadCount; j++) {
        waitMap[i].threads[j]->thread->state = READY;
        enqueue(RRqueue, waitMap[i].threads[j]);
      }
      waitMap[i].id = 0;
    }
  }
  current->thread->state = DEAD;
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

uint8_t open(int i, queueADT * array){
  if(/*i >= MAX_QUEUES || */array[i] != NULL)
    return FAIL;

  array[i] = initQueue();
  if(array[i] == NULL)
    return FAIL;
  return SUCCESS;
}

static queueADT getQueue(int i, int type){
  switch(type){
    case STDIN:
      return stdinQueue;
    case SEM:{
      if(i<0 || i>=MAX_QUEUES)
        return NULL;
      return semQueues[i];
    }
    case READ:{
      if(i<0 || i>=MAX_QUEUES)
        return NULL;
      return rMsgQueues[i];
    }
    case WRITE:{
      if(i<0 || i>=MAX_QUEUES)
        return NULL;
      return wMsgQueues[i];
    }
    case KEY:{
      if(getKeyId(i)<0 || getKeyId(i)>KEYS)
        return NULL;
      return keyQueues[getKeyId(i)];
    }
  }
  return NULL;
}

void block(int i, int type){
  current->thread->state = BLOCKED;
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
  return enqueue(RRqueue, node);
}

static int getKeyId(int key){
  return key - 'a';
}

uint8_t initKey(int key){
  if(key < 'a' || key > 'z')
    return FAIL;
  return open(getKeyId(key), keyQueues);
}

void destroyKey(int key){
  if(key < 'a' || key > 'z')
    return FAIL;
  free(keyQueues[getKeyId(key)]);
  keyQueues[getKeyId(key)] = NULL;
}

uint8_t initMsg(int msgId){
  if(open(msgId,rMsgQueues)==SUCCESS && open(msgId,wMsgQueues)==SUCCESS)
    return SUCCESS;
  return FAIL;
}

void destroyMsg(int msgId){
  if(msgId < MAX_QUEUES || msgId >= 0){
    free(rMsgQueues[msgId]);
    rMsgQueues[msgId] = NULL;
    free(wMsgQueues[msgId]);
    wMsgQueues[msgId] = NULL;
  }
}

uint8_t initSem(int semId){
  return open(semId,semQueues);
}

void destroySem(int semId){
  if(semId < MAX_QUEUES || semId >= 0){
    free(semQueues[semId]);
    semQueues[semId] = NULL;
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
