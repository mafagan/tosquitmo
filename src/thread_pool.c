#include "thread_pool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* task_thread(void *threadid)
{

    return NULL;
}

void thread_pool_init(pthread_t *threads, int num)
{
    int i = 0;
    int rc;
    for(i = 0; i< num; i++){
        rc = pthread_create(&threads[i], NULL, task_thread, NULL);
        if(rc){
            perror("Thread create");
            exit(EXIT_FAILURE);
        }
    }
    return;
}

void thread_pool_destroy(pthread_t *threads, int num)
{

    return;
}
