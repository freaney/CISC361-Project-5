#include "t_lib.h"

//ucontext_t *running;
//ucontext_t *ready;
tcb *running;
tcb *ready;
tcb *head;
int max_id;

void t_yield()
{
  tcb *tmp;

  tmp = running;
  running = ready;
  ready = tmp;

  swapcontext(ready->thread_context, running->thread_context);
}

void t_init()
{
  max_id = 1;
  tcb *tmp;
  tmp = malloc(sizeof(tcb));
  tmp->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
  tmp->next = NULL;
  tmp->thread_id = 0;
  tmp->thread_priority = 0;

  getcontext(tmp->thread_context);    /* let tmp be the context of main() */
  running = tmp;
}

int t_create(void (*fct)(int), int id, int pri)
{
  size_t sz = 0x10000;

  ucontext_t *uc;
  uc = (ucontext_t *) malloc(sizeof(ucontext_t));

  getcontext(uc);
/***
  uc->uc_stack.ss_sp = mmap(0, sz,
       PROT_READ | PROT_WRITE | PROT_EXEC,
       MAP_PRIVATE | MAP_ANON, -1, 0);
***/
  uc->uc_stack.ss_sp = malloc(sz);  /* new statement */
  uc->uc_stack.ss_size = sz;
  uc->uc_stack.ss_flags = 0;
  uc->uc_link = running->thread_context; 
  makecontext(uc, (void (*)(void)) fct, 1, id);
  tcb *new_tcb = malloc(sizeof(tcb));
  tcb->thread_context = uc;
	tcb->thread_id = max_id++;
	tcb->thread_priority = pri;
	tcb->next = NULL;
	tcb *tmp = ready;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}
  tmp->next = new_tcb;
}


void t_terminate(void);

void tcb_free(tcb *thread);
