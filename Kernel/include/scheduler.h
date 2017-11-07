#ifndef SCHED_H
#define SCHED_H

#include <PCBTableADT.h>
#include <MMU.h>

typedef struct{
    int pid;
    int threadCount;
    tcbADT * threads;
} processCDT;

typedef * processCDT processADT;

typedef struct qnode{
        void * elem;
        struct qnode *next;
        struct qnode *prev;
} qnode;

typedef struct{
        qnode *back;
        qnode *front;
}queueCDT;

typedef * queueCDT queueADT;

/*
 * Initialize scheduler.
 */
void initScheduler();

/*
 * Add a new thread into the scheduler.
 * Retunrs 1 on success, otherwise 0.
 */
int addThread(tcbADT);

/*
 * Dispatch the next thread in the queue.
 * If it's a new thread, it fakes an initial state in the new stack.
 */
void dispatchNextThread();

/*
 * Create an empty queue.
 */
static queueADT create();

/*
 * Check if queue is empty.
 * Returns 1 if the queue is empty, and otherwise 0.
 */
static int isEmpty(queueADT);

/*
 * Add element to queue.
 * Returns 1 on success, and otherwise 0.
 */
static int enqueue(queueADT, void*);

/*
 * Remove element from queue, and return it.
 */
static void* dequeue(queueADT);


#endif