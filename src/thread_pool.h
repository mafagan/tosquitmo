#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#include <pthread.h>

#include "tosquitmo.h"
#include "types.h"

void thread_pool_init(data_t *pdata);
void thread_pool_destroy(data_t *pdata);

void* task_thread(void *threadid);

#endif
