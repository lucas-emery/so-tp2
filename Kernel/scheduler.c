#include <scheduler.h>


queueADT RRqueue;
queueADT * semQueues;
processADT * processes;
uint8_t processCount = 0;
uint8_t semCount = 0;
tcbADT current;

int addProcess(int pid){
  processes = realloc(processes,++processCount*sizeof(processADT));
  if(processes == NULL)
    return 0;
  processes[processCount-1] = malloc(sizeof(processCDT));
  processADT aux = processes[processCount-1];
  if(aux == NULL)
    return 0;
  aux->pid = pid;
  aux->threadCount = 0;
  return 1;
}

int addThread(tcbADT t){
  if(t == NULL)
    return 0;
  addThreadToProcess(getProcess(t->pid),t);
  return enqueue(RRqueue, (void*) t);
}

static void addThreadToProcess(processADT p,tcbADT t){
  if(t == NULL || p == NULL)
    return;
  p->threads = realloc(p->threads,++threadCount*sizeof(tcbADT));
  p->threads[threadCount-1] = t;
}

static processADT getProcess(int pid){
  for(int i = 0; i < processCount; i++){
    if(processes[i]->id == pid){
      return processes[i];
    }
  }
}

int getCurrentThread(){
  return current->pid;
}

void dispatchNextProcess(){
  current->state = READY;
  enqueue(RRqueue, (void*) current);
  current = (tcbADT) dequeue(RRqueue);
  int isNew = (current->state == NEW);
  current->state = RUNNING;
  dispatch(current->stack, isNew?executableMemoryAdress:0x0);
}

int initSemaphore(int semId){
    semQueues = realloc(semQueues,(semId+1)*sizeof(queueADT));
    if(!semQueues)
        return 0;
    semQueues[semId] = malloc(sizeof(queueCDT));
    if(!semQueues[semId])
        return 0;
    return 1;
}

void closeSemaphore(int semId){
    free(semQueues[semId]);
    semQueues[semId] = NULL:
}



int initScheduler(){
   RRqueue = initQueue();
   return (RRqueue!=NULL);
}

void semBlock(int semId){
    
}

static queueADT initQueue(){
    queueADT ret = malloc(sizeof(queueCDT));
    if (!ret)
           return NULL;
    ret->back = NULL;
    ret->front = NULL;
    return ret;
}

static int isEmpty(queueADT q){
   if (!q)
           return 1;
   if (!q->front)
           return 1;
   else
           return 0;
}

static int enqueue(queueADT q,void* elem){
   qnode *new = malloc(sizeof(qnode));
   if (!new)
           return 0;
   if (!q || !elem){
           free(new);
           return 0;
   }
   new->elem = elem;
   new->next = q->back;
   new->prev = NULL;
   if (q->back)
           q->back->prev = new;
   q->back = new;
   if (!q->front)
           q->front = new;
   return 1;
}

static void* dequeue(queueADT q){
   qnode *prev;
   void* elem;
   if (isempty(q))
           return NULL;
   prev = q->front->prev;
   elem = q->front->elem;
   free(q->front);
   q->front = prev;
   return elem;
}