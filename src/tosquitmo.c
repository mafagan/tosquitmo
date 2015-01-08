#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ev.h>
#include <pthread.h>
#include <unistd.h>

#include "net.h"
#include "config.h"
#include "net_read_handle.h"
#include "signal_handle.h"
#include "memory_pool.h"
#include "tosquitmo.h"
#include "thread_pool.h"

extern config_t config;

static void data_init(data_t *pdata)
{

    pdata->session_head = NULL;
    pdata->global_data = NULL;
    pdata->reactor = EV_DEFAULT;
}

static void data_destroy()
{
    return;
}

int main(int argv, char **argc)
{
    data_t pdata;
    data_init(&pdata);
    config_init(&pdata);
    memory_pool_init();
    pthread_t *threads = (pthread_t*)talloc(config.thread_num*sizeof(pthread_t));
    thread_pool_init(threads, config.thread_num);

    global_data->listenfd = new_tcp_listensock();

    net_init(global_data->reactor, global_data->listenfd);
    signal_init(reactor);
    ev_run(reactor, 0);

    thread_pool_destroy(threads, config.thread_num);
    data_destroy();
    return 0;
}
