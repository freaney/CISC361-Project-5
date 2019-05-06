#include "t_lib.h"
#include "ud_thread.h"

//ucontext_t *running;
//ucontext_t *ready;
tcb *running;
tcb *ready;
tcb *head;

void t_yield()
{
	if (ready !=NULL){
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
		swapcontext(ready->thread_context, running->thread_context);
	}
}

void t_init()
{
	tcb *tmp = malloc(sizeof(tcb));
	tmp->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
	tmp->next = NULL;
	tmp->thread_id = 0;
	tmp->thread_priority = 0;

	getcontext(tmp->thread_context);    /* let tmp be the context of main() */
	running = tmp;
	ready = NULL;
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
	new_tcb->thread_context = uc;
	new_tcb->thread_id = id;
	new_tcb->thread_priority = pri;
	new_tcb->next = NULL;
	tcb *tmp = ready;
	if (tmp == NULL)
		ready = new_tcb;
	else{
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = new_tcb;
	}
}


void t_shutdown(void) {
	tcb *tmp = ready;
	while (tmp->next != NULL) {
		tcb *last = tmp;
		tmp = tmp->next;
		free(last);
	}
	free(tmp);
	free(running);
}

void t_terminate(void) {
	tcb *curr = running;
	setcontext(ready->thread_context);
	running = ready;
	ready = ready->next;
	free(curr);
}
void tcb_free(tcb *thread);
