#ifndef SCHED_H
#define SCHED_H

#include <stdint.h>
#include <pcb.h>
#include <MMU.h>
#include <thread.h>
#include <lib.h>

#define IDLE 0
#define KEYS 'z' - 'a' +1

typedef struct qnode{
        tcbADT thread;
        struct qnode *next;
        struct qnode *prev;
} qnode;

typedef struct{
        qnode *back;
        qnode *front;
}queueCDT;

typedef queueCDT * queueADT;

/*
 *	Author: stu
 *	# -> defined constant
 *	$ -> global variable
 *	@ -> function parameter
 */


uint8_t initKey(int key);
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
 *	Sets state of $current to #DEAD yields.
 */
void killThread();

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
 *	Opens a queue at index @i of @array.
 *	Paramaters: index, array
 *  Returns #SUCCESS on success, otherwise #FAIL.
 */
uint8_t open(int index, queueADT * array);

/*
 *	Blocks $current thread and offers it to the queue where it belongs depending on @type and @index.
 *	Paramaters: index, type
 */
void block(int index, int type);

/*
 *	Unblocks thread at end of the queue at @index of the array chosen by type and queues it at $RRqueue.
 *	Paramaters: index, type
 *  Returns #SUCCESS on success, otherwise #FAIL.
 */
uint8_t unblock(int index, int type);

#endif
