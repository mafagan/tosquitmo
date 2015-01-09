#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "thread_pool.h"
#include "tosquitmo.h"

void* task_thread(void *threadid)
{

    return NULL;
}

void thread_pool_init(data_t *pdata)
{
    int i = 0;
    int rc;
    for(i = 0; i< pdata->config->thread_num; i++){
        rc = pthread_create(&(pdata->threads[i]), NULL, task_thread, NULL);
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
