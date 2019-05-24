/* #include "t_lib.h" */
#include "ud_thread.h"

// ucontext_t *running;
// ucontext_t *ready;
tcb *running;
tcb *ready;

// relinqueshes CPU to thread on running queue, moves new thread to running,
// moves old running to end of ready
void t_yield() {
  if (ready != NULL) {
    tcb *tmp, *tmp2;
    tmp = running;
    running = ready;
    tmp2 = ready;
    while (tmp2->next != NULL) {
      tmp2 = tmp2->next;
    }
    tmp2->next = tmp;
    ready = ready->next;
    running->next = NULL;
    tmp2 = ready;
    if (tmp2 != NULL)
      while (tmp2->next != NULL)
        tmp2 = tmp2->next;
    swapcontext(tmp2->thread_context, running->thread_context);
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
}

void sem_wait(sem_t *s) {
  while (s->count < 1){
    t_yield();
  }
  s->count --;
}

void sem_signal(sem_t *s) {
  s->count ++;
}

void sem_destroy(sem_t **s)
{

}

/////////////////////////////// Messaging /////////////////////////////////////
int mbox_create(mbox **mb) {
  *mb = malloc(sizeof(mbox));
  (*mb)->msg = malloc(sizeof(struct messageNode));
  (*mb)->mbox_sem = malloc(sizeof(sem_t));
}

// going to assume that sender and receiver are both the running thread's id
void mbox_deposit(mbox *mb, char *msg, int len) {
  
}
