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
#include "logging.h"

data_t pdata;

static void data_init(data_t *pdata)
{

    pdata->session_head = NULL;
    pdata->config = NULL;
    pdata->reactor = EV_DEFAULT;
    pdata->session_end = NULL;
    pdata->threads = NULL;
}

static void data_destroy()
{
    log_info("data destroy");
    return;
}

int main(void)
{
    data_init(&pdata);
    config_init(&pdata);

    log_init();

    memory_pool_init(&pdata);

    signal_init(&pdata);

    thread_pool_init(&pdata);

    net_init(&pdata);


    ev_run(pdata.reactor, 0);

    thread_pool_destroy(&pdata);
    data_destroy();
    return 0;
}
