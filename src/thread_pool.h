#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#include <pthread.h>

void thread_pool_init(pthread_t *threads, int num);
void thread_pool_destroy(pthread_t *threads, int num);

void* task_thread(void *threadid);

#endif
