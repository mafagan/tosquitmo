#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "thread_pool.h"
#include "tosquitmo.h"
#include "logging.h"
#include "memory_pool.h"
#include "database.h"
#include "tosquitmo_broker.h"

extern data_t pdata;

/* thread to exec command
 * get msg from msg_queue,
 * exec and remove it from
 * queue.
 */

void* task_thread(void *threadid)
{
    int thread_id = (int)threadid;

    log_info("thread %d createed", thread_id);

    pthread_mutex_lock(&pdata.ctrl_flag_lock);
    while(pdata.control_flag){
        pthread_mutex_unlock(&pdata.ctrl_flag_lock);

        //TODO whether to sleep or not judged by cpu payload
        usleep(1);
        tos_exec_cmd(pdata.msg_queue);

        pthread_mutex_lock(&pdata.ctrl_flag_lock);
    }
    pthread_mutex_unlock(&pdata.ctrl_flag_lock);
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
        rc = pthread_create(pdata->threads+i, NULL, task_thread, (void*)i);
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
