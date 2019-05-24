/* 
 * Test Program #10 - Critical Sections with Yields
 */

#include <stdio.h>
#include <stdlib.h>
#include "ud_thread.h"

sem_t *mutex;
int resource = 0;

void read_function(int val) 
{
  printf("I am thread %d...\n", val);
fflush(stdout);
  sem_wait(mutex);
    printf("thread %d entering 1st CS...\n", val);
    fflush(stdout);
	resource = 11;
    printf("in thread: resource is %d\n", resource);
    fflush(stdout);
	t_yield();
    resource = 22;
    printf("in thread: resource is %d\n", resource);
    printf("thread %d leaving 1st CS...\n", val);
    fflush(stdout);
  sem_signal(mutex);

  sem_wait(mutex);
    printf("thread %d entering 2nd CS...\n", val);
    fflush(stdout);
    resource = 55;
    printf("in thread: resource is %d\n", resource);
    fflush(stdout);
    t_yield();
    resource = 66;
    printf("in thread: resource is %d\n", resource);
    printf("thread %d leaving 2nd CS...\n", val);
    fflush(stdout);
  sem_signal(mutex);

  printf("thread %d: t_terminate()\n", val);
    fflush(stdout);
  t_terminate();
}

int main(void)
{
   t_init();
   sem_init(&mutex, 1);

   t_create(read_function, 123, 1);

   t_yield();

   sem_wait(mutex);
     printf("main thread entering CS...\n");
    fflush(stdout);
     resource = 33;
     printf("in main: resource is %d\n", resource);
    fflush(stdout);
     t_yield();
     resource = 77;
     printf("in main: resource is %d\n", resource);
     printf("main thread leaving CS...\n");
    fflush(stdout);
   sem_signal(mutex);

   printf("main thread gives up CPU...\n");
    fflush(stdout);
   t_yield();
   t_yield();

   printf("in main: sem_destroy() and t_shutdown()\n");
    fflush(stdout);
   sem_destroy(&mutex);
   t_shutdown();

   return 0;
}
