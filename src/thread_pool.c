#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "thread_pool.h"
#include "tosquitmo.h"
#include "logging.h"
#include "memory_pool.h"

void* task_thread(void *threadid)
{
    log_info("thread %lu createed", pthread_self());
    return NULL;
}

void thread_pool_init(data_t *pdata)
{
    int i = 0;
    int rc;

    pdata->threads = (pthread_t*)talloc(sizeof(pthread_t)*pdata->config->thread_num);

    if(!pdata->threads){
        exit(EXIT_FAILURE);
    }

    for(i = 0; i< pdata->config->thread_num; i++){
        rc = pthread_create(pdata->threads+i, NULL, task_thread, NULL);
        if(rc){
            perror("Thread create");
            exit(EXIT_FAILURE);
        }
    }
    return;
}

void thread_pool_destroy(data_t *pdata)
{

    return;
}
