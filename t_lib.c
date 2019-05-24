/*
 * t_lib.c
 *
 * Authors: Sophia Freaney and Connor Onweller
 *
 * Purpose: Implements function to initialize thread library,
 * 			create/terminate threads, and use semaphores. Struct
 * 			descriptions are in t_lib.h, and extended function
 * 			descriptions are in ud_thread.c
 *
 */

#include "ud_thread.h"
#include <string.h>

tcb *running;
tcb *ready;
struct mboxList *box_list;

// relinqueshes CPU to thread on running queue, moves new thread to running,
// moves old running to end of ready
void t_yield() {
  if (ready != NULL) {
    tcb *old, *new, *tmp;
    old = running;
	new = ready;
	// Goes to end of ready queue and adds currently running thread, which will be
	// deactivated at end of function. Also dequeues the new thread to be run from
	// the head of the ready thread.
    tmp = ready;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = old;
	ready = ready->next;
    new->next = NULL;
	running = new;
	swapcontext(old->thread_context, new->thread_context);
  }
}

// sets up first thread (main) sets up running and ready queues
void t_init() {
  tcb *tmp = malloc(sizeof(tcb));
  tmp->thread_context = (ucontext_t *)malloc(sizeof(ucontext_t));
  tmp->next = NULL;
  tmp->thread_id = 0;
  tmp->thread_priority = 0;

  getcontext(tmp->thread_context); /* let tmp be the context of main() */
  running = tmp;
  ready = NULL;
  box_list = NULL;
}

int t_create(void (*fct)(int), int id, int pri) {
  /* printf("created %d\n", id); */
  size_t sz = 0x10000;
  ucontext_t *uc;
  uc = (ucontext_t *)malloc(sizeof(ucontext_t));

  getcontext(uc);
  uc->uc_stack.ss_sp = malloc(sz); /* new statement */
  uc->uc_stack.ss_size = sz;
  uc->uc_stack.ss_flags = 0;
  uc->uc_link = running->thread_context;

  makecontext(uc, (void (*)(void))fct, 1, id);
  tcb *new_tcb = malloc(sizeof(tcb));
  new_tcb->thread_context = uc;
  new_tcb->thread_id = id;
  new_tcb->thread_priority = pri;
  new_tcb->next = NULL;
  tcb *tmp = ready;
  if (tmp == NULL)
    ready = new_tcb;
  else {
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = new_tcb; // add to end of ready list
  }

  mbox *mb;
  mbox_create(&mb);
  if (box_list == NULL) {
    box_list = malloc(sizeof (struct mboxList));
    box_list->mbox = mb;
    box_list->tid = id;
    box_list->next = NULL;
  }
  else {
    struct mboxList *cur_box = box_list;
    while (cur_box->next !=NULL) {
      cur_box = cur_box->next;
    }
    cur_box->next = malloc(sizeof(struct mboxList));
    cur_box->next->mbox = mb;
    cur_box->next->tid = id;
  }
}

// frees all threads
void t_shutdown(void) {

  if (ready != NULL) {
    tcb *tmp = ready;
    while (tmp->next != NULL) {
      tcb *last = tmp;
      tmp = tmp->next;
      tcb_free(last); // free contex
      free(last);     // free thread
    }
    free(tmp);
  }
  /* free(running->thread_context); */
  tcb_free(running); // free context
  free(running);     // free thread
}

// removes and frees running thread
void t_terminate(void) {
  running->next = NULL; // make sure its the only one in the list
  tcb_free(running);    // free the context
  free(running);        // free the thread
  running = ready;      // update running to a ready thread
  ready = ready->next;  // remove last head of ready
  running->next = NULL; // only one running
  setcontext(running->thread_context);
}

// Frees the thread's stack and context
void tcb_free(tcb *thread) {
  free(thread->thread_context->uc_stack.ss_sp);
  free(thread->thread_context);
}

////////////////////////////////// SEMAPHORES /////////////////////////////////
int sem_init(sem_t **sp, unsigned int sem_count) {
  *sp = malloc(sizeof(sem_t));
  (*sp)->count = sem_count;
  (*sp)->q = NULL;
  return sem_count;
}

void sem_wait(sem_t *s) {
	while (s->count < 1) {
		tcb *old, *new;
		old = running;
		new = ready;
		running = new;
		ready = ready->next;
		// Enqueues previously running thread to semaphore queue
		tcb *tmp = s->q;
		if (tmp == NULL) {
			s->q = old;
		} else {
			while (tmp->next != NULL) {
				tmp = tmp->next;
			}
			tmp->next = old;
		}
		new->next = NULL;
		// Activates new thread
		swapcontext(old->thread_context, new->thread_context);
	}
	s->count--;
}

void sem_signal(sem_t *s) {
  s->count++;
  if (s->q != NULL) {
	  // Dequeues a thread from the waiting queue, and adds it to the ready queue
	  tcb *tmp = ready;
	  tcb *tmp2 = s->q;
	  s->q = s->q->next; // Removes thread from semaphore queue
	  tmp2->next = NULL;
	  if (tmp == NULL) {
		  ready = tmp2;
	  } else {
		  while (tmp->next != NULL) {
			  tmp = tmp->next;
		  }
		  tmp->next = tmp2; // add thread to ready queue
	  }
  }
}

void sem_destroy(sem_t **s)
{
	free(*s);
}

/////////////////////////////// MESSAGING /////////////////////////////////////

int mbox_create(mbox **mb) {
  *mb = malloc(sizeof(mbox));
  /* (*mb)->msg = malloc(sizeof(struct messageNode)); */
  (*mb)->msg = NULL;
  (*mb)->mbox_sem = malloc(sizeof(sem_t));
  sem_init(&((*mb)->mbox_sem), 1);
}

void mbox_destroy(mbox **mb) {
}

// going to assume that sender and receiver are both the running thread's id
void mbox_deposit(mbox *mb, char *msg, int len) {
  struct messageNode *newMsg = malloc(sizeof(struct messageNode));
  newMsg->sender = running->thread_id;
  newMsg->receiver = running->thread_id; //PROBABLY NEED TO CHANGE THIS LATER
  newMsg->next = NULL;
  newMsg->message = malloc(sizeof(char*)*(1+strlen(msg)));
  strcpy(newMsg->message, msg);
  newMsg->len = len;

  sem_wait(mb->mbox_sem);
  struct messageNode *cur = mb->msg;
  if (cur == NULL) {
    mb->msg = newMsg;
  }
  else {
    while (cur->next != NULL) {
      cur = cur->next;
    }
    cur->next = newMsg;
  }
  sem_signal(mb->mbox_sem);
}

void mbox_depositV2(mbox *mb, char *msg, int len, int tid) {
  struct messageNode *newMsg = malloc(sizeof(struct messageNode));
  newMsg->sender = running->thread_id;
  newMsg->receiver = tid; //PROBABLY NEED TO CHANGE THIS LATER
  newMsg->next = NULL;
  newMsg->message = malloc(sizeof(char*)*(1+strlen(msg)));
  strcpy(newMsg->message, msg);
  newMsg->len = len;

  sem_wait(mb->mbox_sem);
  struct messageNode *cur = mb->msg;
  if (cur == NULL) {
    mb->msg = newMsg;
  }
  else {
    while (cur->next != NULL) {
      cur = cur->next;
    }
    cur->next = newMsg;
  }
  sem_signal(mb->mbox_sem);
}
void mbox_withdraw(mbox *mb, char *msg, int *len) {
  if (mb->msg == NULL)
    return;
  sem_wait(mb->mbox_sem);
  strcpy(msg, mb->msg->message);
  *len = mb->msg->len;
  struct messageNode *tmp = mb->msg;
  mb->msg = mb->msg->next;
  free(tmp->message);
  free(tmp);
  sem_signal(mb->mbox_sem);
}

void mbox_withdrawV2(mbox *mb, char *msg, int *len, int *tid) {
  if (mb->msg == NULL)
    return;
  sem_wait(mb->mbox_sem);
  strcpy(msg, mb->msg->message);
  *tid = mb->msg->sender;
  *len = mb->msg->len;
  struct messageNode *tmp = mb->msg;
  mb->msg = mb->msg->next;
  free(tmp->message);
  free(tmp);
  sem_signal(mb->mbox_sem);
}

void mbox_withdrawV3(mbox *mb, char *msg, int *len, int tid) {
  if (mb->msg == NULL)
    return;
  sem_wait(mb->mbox_sem);
  struct messageNode *curmsg = mb->msg;
  struct messageNode *prev = NULL;
  while (curmsg != NULL) {
    if (curmsg->sender == tid){
      strcpy(msg, curmsg->message);
      *len = curmsg->len;
      struct messageNode *tmp = curmsg;
      curmsg = curmsg->next;
      if (prev !=NULL) {
        prev->next = curmsg;
      }
      /* free(tmp->message); */
      /* free(tmp); */
      break;
    }
    /* printf("%s\n", curmsg->message); */
    prev = curmsg;
    curmsg = curmsg->next;
  }
  sem_signal(mb->mbox_sem);
}

void send(int tid, char *msg, int len) {
  mbox box;
  struct mboxList *tmp = box_list;
  while (tmp != NULL) {
    if (tmp->tid == tid) {
      mbox_depositV2(tmp->mbox, msg, len, tid);
      return;
    }
    tmp = tmp->next;
  }
  printf("Error: no possible receivers\n");
}

void receive(int *tid, char *msg, int *len) {
  mbox box;
  struct mboxList *tmp = box_list;
  while (tmp != NULL) {
    /* printf("current box is %d\n", tmp->tid); */
    if (tmp->tid == running->thread_id) {
      if (*tid == 0)
        mbox_withdrawV2(tmp->mbox, msg, len, tid);
      else
        mbox_withdrawV3(tmp->mbox, msg, len, *tid);
        /* mbox_withdrawV2(tmp->mbox, msg, len, tid); */

      return;
    }
    tmp = tmp->next;
  }
  /* printf("msg = %s\n", msg); */
  printf("Error: no possible receivers\n");
}
