#ifndef SCHED_H
#define SCHED_H

#include <process.h>
#include <MMU.h>
#include <thread.h>
#include <lib.h>

typedef struct{
    int count;
    tcbADT * threads;
} threadPackCDT;

typedef struct qnode{
        void * elem;
        struct qnode *next;
        struct qnode *prev;
} qnode;

typedef struct{
        qnode *back;
        qnode *front;
}queueCDT;

typedef * threadPackCDT threadPackADT;
typedef * queueCDT queueADT;

/*
 *	Author: stu
 *	# -> defined constant
 *	$ -> global variable
 *	@ -> function parameter
 */


/*
 * 	Initializes $RRqueue.
 *	Return: #SUCCESS on success, otherwise #FAIL.
 */
uint8_t initScheduler();

/*
 *	Sets state of @thread to NEW and queues @thread into $RRqueue.
 *	Parameters: thread
 *	Return: #SUCCESS on success, otherwise #FAIL.
 */
uint8_t addThread(tcbADT thread);

/*
 *	Enqueues $current and sets $current to the next thread in the queue.
 *	Sets system context $current's context.
 */
void schedule();

/*
 *	Returns the pid of $current.
 *	Return: $current.pid
 */
int getCurrentProcess();

/*
 * Called upon the opening of a semaphore to initialize the queue used for the event.
 * Paramaters: semId
 * Returns #SUCCESS on success, otherwise #FAIL.
 */
uint8_t semOpen(int semId);

/*
 *	Called upon the closing of a semaphore to destroy the queue used for the event.
 *	Parameters: semId
 */
void semClose(int semId);

/*
 * Blocks all of the running process´ threads, packing and queueing them together into the corresponding semQueue.
 * Paramaters: semId
 * Returns #SUCCESS on success, otherwise #FAIL.
 */
uint8_t semBlock(int semId);

/*
 * Dequeues a pack of threads from the corresponding semQueue, unpacking and queueing them into the RRqueue.
 * Paramaters: semId
 * Returns #SUCCESS on success, otherwise #FAIL.
 */
uint8_t semUnblock(int semId);

/*
 *	If @type is BLOCK, it removes all threads in @pack from the queue and sets their state to BLOCKED.
 *	If @type is UNBLOCK, it enqueues all threads in @pack
 *	Paramaters: pack, type
 */
static void manageThreads(packADT pack,uint8_t type);

/*
 * Create an empty queue.
 */
static queueADT create();

/*
 * Check if queue is empty.
 * Returns #SUCCESS if the queue is empty, and otherwise #FAIL.
 */
static int isEmpty(queueADT);

/*
 * Add element to queue.
 * Returns #SUCCESS on success, and otherwise #FAIL.
 */
static int enqueue(queueADT, void*);

/*
 * Remove last element from queue, and return it.
 */
static void* dequeue(queueADT);

/*
 * Remove element from queue, using a static recursive function
 */
static void remove(queueADT,void*);

#endif