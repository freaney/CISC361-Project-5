#include "t_lib.h"
/*
 * thread library function prototypes
 */

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
 *end of the ready queue. The first thread in the ready queue resumes execution
 *(if there is one).
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

int sem_init(sem_t **sp, unsigned int count);
void sem_wait(sem_t *sp);
void sem_signal(sem_t *sp);
void sem_destroy(sem_t **sp);


int mbox_create(mbox **mb);

int mbox_create(mbox **mb);
void mbox_destroy(mbox **mb);
void mbox_deposit(mbox *mb, char *msg, int len);
void mbox_withdraw(mbox *mb, char *msg, int *len);
void send(int tid, char *msg, int len);
void receive(int *tid, char *msg, int *len);
