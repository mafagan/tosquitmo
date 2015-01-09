#include <ev.h>

#include "signal_handle.h"
#include "memory_pool.h"

static void sigint_cb(struct ev_loop *reactor, ev_signal *int_w, int revents)
{

    return;
}

static void sigpipe_cb(struct ev_loop *reactor, ev_signal *int_w, int revents)
{

    return;
}

void signal_init(data_t *pdata)
{
    ev_signal int_w;
    ev_signal_init(&int_w, sigint_cb, SIGINT);
    ev_signal_start(pdata->reactor, &int_w);

    ev_signal_init(&int_w, sigpipe_cb, SIGPIPE);
    ev_signal_start(pdata->reactor, &int_w);

    return;
}


