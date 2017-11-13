#ifndef SCHED_H
#define SCHED_H

#include <stdint.h>
#include <pcb.h>
#include <MMU.h>
#include <thread.h>
#include <lib.h>

typedef struct{
    int count;
    tcbADT * threads;
} threadPackCDT;

typedef struct qnode{
        tcbADT thread;
        struct qnode *next;
        struct qnode *prev;
} qnode;

typedef struct{
        qnode *back;
        qnode *front;
}queueCDT;

typedef threadPackCDT * threadPackADT;
typedef queueCDT * queueADT;

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
 * Called upon the opening of a message to initialize the queue used for the event.
 * Paramaters: msgId
 * Returns #SUCCESS on success, otherwise #FAIL.
 */
uint8_t initMsg(int msgId);

/*
 *	Called upon the closing of a message to destroy the queue used for the event.
 *	Parameters: msgId
 */
void destroyMsg(int msgId);

/*
 *	If @blocking is #TRUE, it blocks all of the running process´ threads,
 *	packing and queueing them together into the corresponding msgQueue, depending on @type.
 *	Otherwise, it packs only the running thread.
 *	Paramaters: msgId, type, blocking
 */
//void msgBlock(int msgId, int type, int blocking);

/*
 *	Dequeues a pack of threads from the corresponding msgQueue, depending on @type,
 *	unpacking and queueing them into $RRqueue.
 *	Paramaters: msgId, type
 *	Returns #SUCCESS on success, otherwise #FAIL.
 */
//uint8_t msgUnblock(int msgId, int type);

/*
 * Called upon the opening of a semaphore to initialize the queue used for the event.
 * Paramaters: semId
 * Returns #SUCCESS on success, otherwise #FAIL.
 */
uint8_t initSem(int semId);

/*
 *	Called upon the closing of a semaphore to destroy the queue used for the event.
 *	Parameters: semId
 */
void destroySem(int semId);

/*
 *	If @blocking is #TRUE, it blocks all of the running process´ threads,
 *	packing and queueing them together into the corresponding semQueue.
 *	Paramaters: semId, blocking
 */
//void semBlock(int semId, int blocking);

/*
 * Dequeues a pack of threads from the corresponding semQueue, unpacking and queueing them into the $RRqueue.
 * Paramaters: semId
 * Returns #SUCCESS on success, otherwise #FAIL.
 */
//uint8_t semUnblock(int semId);

/*
 *	Opens a queue at index @i of @array.
 *	Paramaters: i, array
 */
uint8_t open(int i, queueADT ** array);

/*
 *	Blocks $current thread and offers it to the queue at index @i of @array.
 *	If @blocking is #TRUE
 *	Paramaters: i, array, blocking
 */
//static void block(int i, queueADT * array,int blocking);

/*
 *	Unblocks thread at end of the queue at index @i of @array and queues it at $RRqueue.
 *	Paramaters: i, array
 */
//static uint8_t unblock(int i, queueADT * array);

/*
 *	If @type is BLOCK, it removes all threads in @pack from the queue and sets their state to BLOCKED.
 *	If @type is UNBLOCK, it enqueues all threads in @pack
 *	Paramaters: pack, type
 */
//static void managePack(threadPackADT pack,uint8_t type);

/*
 * Create an empty queue.
 */
queueADT initQueue();

/*
 * Check if queue is empty.
 * Returns #SUCCESS if the queue is empty, and otherwise #FAIL.
 */
int isEmpty(queueADT);

/*
 * Add element to queue.
 * Returns #SUCCESS on success, and otherwise #FAIL.
 */
uint8_t enqueue(queueADT, qnode*);

/*
 * Remove last element from queue, and return it.
 */
qnode* dequeue(queueADT);

/*
 * Remove element from queue, using a static recursive function
 */
void remove(queueADT,void*);

#endif
