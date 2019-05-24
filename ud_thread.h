/*
 * ud_thread.h
 *
 * Authors: Sophia Freaney and Connor Onweller
 *
 * Purpose: Declares and describes thread library functions
 * 			as well as semaphore functions
 */

#include "t_lib.h"

/*
 * Function: t_create
 * ------------------
 *  Creates a thread with priority 'pri' running the function 'fct' with thread
 *id of 'id' Creates a tcb representation of the new thread and adds it to the
 *end of the ready queue
 *
 *  Returns: id
 */
int t_create(void (*fct)(int), int id, int pri);

/*
 * Function: t_yield
 * ------------------
 *	The calling thread voluntarily relinquishes the CPU and is placed at the
 *  end of the ready queue. The first thread in the ready queue resumes execution
 *  (if there is one).
 *
 *  Returns: void
 */
void t_yield(void);

/*
 * Function: t_init
 * ------------------
 *	Initializes the thread library by setting up the running and the ready
 *queues; it initializes the running queue with a tcb representation of the main
 *thread.
 *
 *	Return: void
 *
 */
void t_init(void);

/*
 * Function: t_shutdown
 * ------------------
 *  Shuts down the thread library and frees all the dynamically allocated memory
 * for all threads in the tcb queues
 *
 *  Return: void
 */
void t_shutdown(void);

/*
 * Function: t_terminate
 * ------------------
 *  Terminates the calling thread by removing and freeing its tcb from the
 * running queue while resuming the execution of the thread with the thread at
 * the head of the ready queue
 *
 *  Return: void
 */
void t_terminate(void);

///////////////////////////////// SEMAPHORES //////////////////////////////////

/*
 * Function: sem_init
 * ------------------
 *  Initializes semaphore and its parameters: its count (int) and a queue of waiting
 *  threads (*tcb)
 *  
 *  sp: pointer to semaphore to be initialized
 *  count: initial int value of sp
 *
 *  Returns: sem_count (int)
 *
 */
int sem_init(sem_t **sp, unsigned int count);

/*
 * Function: sem_wait
 * ------------------
 * 	If the semaphore count is less than 1, it adds the current running thread to the waiting
 * 	queue pointed to by the semaphore and dequeues a new thread from the ready queue
 * 	to be run. Once the semaphore count is at least one, the semaphore is decremented by 1
 *	
 *	sp: semaphore used in thread synchronization
 *
 *  Returns: void
 *
 */
void sem_wait(sem_t *sp);

/*
 * Function: sem_signal
 * ------------------
 *  Increments semaphore by 1, then wakes up a thread (if there is one available) by
 *  removing it from the waiting queue and adding it to the ready queue
 *
 *	sp: semaphore used in thread synchronization
 *
 *  Returns: void
 *
 */
void sem_signal(sem_t *sp);

/*
 * Function: sem_destroy
 * ------------------
 *  Frees semaphore memory
 *	
 *	sp: pointer to semaphore used in thread synchronization
 *
 *  Returns: void
 *
 */
void sem_destroy(sem_t **sp);
