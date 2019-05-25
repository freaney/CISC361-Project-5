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

////////////////////////////// MESSAGING //////////////////////////////////////
/*
 * Function: mbox_create
 * ------------------
 *  Creates a message box, which contains a message string and a semaphore
 *	
 *	mb: pointer to mbox pointer to be created
 *
 *  Returns: int
 *
 */
int mbox_create(mbox **mb);

/*
 * Function: mbox_destroy_helper
 * ------------------
 *  Assists mbox destroy by deleting the mbox and its message list
 *	
 *	mb: mbox to be destroyed
 *
 *  Returns: void
 *
 */
void mbox_destroy_helper(mbox *mb);

/*
 * Function: mbox_destroy
 * ------------------
 *  Deletes the mbox_list as well as the input mbox, which can be called from main
 *	
 *	mb: pointer to mbox to be destroyed
 *
 *  Returns: void
 *
 */
void mbox_destroy(mbox **mb);

/*
 * Function: mbox_deposit
 * ------------------
 *  Sends message from one thread to another
 *	
 *	mb: mbox to send message to
 *	msg: string of message
 *	len: length of msg
 *
 *  Returns: void
 *
 */
void mbox_deposit(mbox *mb, char *msg, int len);

/*
 * Function: mbox_withdraw
 * ------------------
 *  Removes message from mbox and deletes the mbox
 *	
 *	mb: mbox from which to obtain message
 *	msg: message obtained
 *	len: length of message obtained
 *
 *  Returns: void
 *
 */
void mbox_withdraw(mbox *mb, char *msg, int *len);

/*
 * Function: send
 * ------------------
 *  Send a message to a thread
 *	
 *	tid: id of thread to be sent message
 *	msg: message to send
 *	len: length of msg
 *
 *  Returns: void
 *
 */
void send(int tid, char *msg, int len);

/*
 * Function: receive
 * ------------------
 * Wait for and receive a message from another thread.
 * The caller has to specify the sender's tid in tid, or sets tid to 0
 * if it intends to receive a message sent by any thread. If there is no
 * "matching" message to receive, the calling thread waits 
 *	
 *	tid: id of matching thread
 *	msg: message received
 *	len: length of msg
 *
 *  Returns: void
 *
 */
void receive(int *tid, char *msg, int *len);
