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
}; typedef struct tcb tcb;

typedef struct {
  int count;
  tcb *q;
} sem_t;

/*
 * Function: tcb_free
 * -------------------
 *  Helper function for t_terminate() and t_shutdown() to free memory from struct tcb
 *
 *  Returns: void
 */
void tcb_free(tcb *thread);
