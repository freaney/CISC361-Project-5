#include "t_lib.h"
#include "ud_thread.h"

//ucontext_t *running;
//ucontext_t *ready;
tcb *running;
tcb *ready;

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
		tmp2 = ready;
		if (tmp2 !=NULL)
			while (tmp2->next != NULL) 
				tmp2 = tmp2->next;
		swapcontext(tmp2->thread_context, running->thread_context);

		/* tmp = ready; */
		/* while (tmp != NULL){ */
		/* 	printf("%d -> ", tmp->thread_id); */
		/* 	tmp = tmp->next; */
		/* } */
		/* printf("NULL\n"); */
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
	/* printf("created %d\n", id); */
	size_t sz = 0x10000;
	ucontext_t *uc;
	uc = (ucontext_t *) malloc(sizeof(ucontext_t));

	getcontext(uc);
	  /* uc->uc_stack.ss_sp = mmap(0, sz, */
	  /* PROT_READ | PROT_WRITE | PROT_EXEC, */
	  /* MAP_PRIVATE | MAP_ANON, -1, 0); */
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
	/* tmp = ready; */
	/* while (tmp != NULL){ */
	/* 	printf("%d -> ", tmp->thread_id); */
	/* 	tmp = tmp->next; */
	/* } */
	/* printf("NULL\n"); */
}


void t_shutdown(void) {

	if (ready != NULL) {
		tcb *tmp = ready;
		while (tmp->next != NULL) {
			tcb *last = tmp;
			tmp = tmp->next;
			tcb_free(last);
			free(last);
		}
		free(tmp);
	}
	/* free(running->thread_context); */
	tcb_free(running);
	free(running);
}

void t_terminate(void) {
	/* tcb *tmp = running; */
	/* running->next = NULL; */
	/* running = ready; */
	/* ready = ready->next; */
	/* /1* running->next = NULL; *1/ */
	/* setcontext(running->thread_context); */
	/* tcb_free(tmp); */
	/* free(tmp); */
	running->next = NULL; // make sure
	tcb_free(running);
	free(running);
	running = ready;
	ready = ready->next;
	running->next = NULL;
	setcontext(running->thread_context);
}
void tcb_free(tcb *thread) {
	free(thread->thread_context->uc_stack.ss_sp);
	/* free(thread->thread_context->uc_stack); */
	free(thread->thread_context);
}
