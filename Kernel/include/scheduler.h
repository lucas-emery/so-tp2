#ifndef SCHED_H
#define SCHED_H

#include <PCBTableADT.h>
#include <MMU.h>
#include <PCBTableADT.h>

typedef struct qnode
{
        tcbADT elem;
        struct qnode *next;
        struct qnode *prev;
} qnode;

typedef struct
{
        qnode *back;
        qnode *front;
}queueCDT;

typedef * queueCDT queueADT;

queueADT queue;

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
 * Dispatch the next process in the queue.
 * If it's a new process, it fakes an initial state.
 */
void dispatchNextProcess();

/*
 * Check if queue is empty.
 * Returns 1 if the queue is empty, and otherwise 0.
 */
static int isEmpty();

/*
 * Add element to queue.
 * Returns 1 on success, and otherwise 0.
 */
static int enqueue(tcbADT elem);

/*
 * Remove element from queue, and return it.
 */
static tcbADT dequeue();


#endif
