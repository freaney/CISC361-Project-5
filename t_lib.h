/*
 * t_lib.h
 *
 * Authors: Sophia Freaney and Connor Onweller
 *
 * Purpose: Houses tcb struct, sem_t struct, and
 * 			tcb_free function
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>

struct tcb {
	int         thread_id;
	int         thread_priority;
	ucontext_t *thread_context;
	struct tcb *next;
  struct mbox       *mbox;
}; typedef struct tcb tcb;

typedef struct {
  int count;
  tcb *q;
} sem_t;

struct messageNode {
  char *message;     // copy of the message 
  int  len;          // length of the message 
  int  sender;       // TID of sender thread 
  int  receiver;     // TID of receiver thread 
  struct messageNode *next; // pointer to next node 
};

struct mbox{
  struct messageNode  *msg;       // message queue
  sem_t               *mbox_sem;
}; typedef mbox mbox;

/*
 * Function: tcb_free
 * -------------------
 *  Helper function for t_terminate() and t_shutdown() to free memory from struct tcb
 *
 *  Returns: void
 */
void tcb_free(tcb *thread);
