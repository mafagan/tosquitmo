#include <ev.h>

#include "signal_handle.h"
#include "memory_pool.h"
#include "logging.h"

static void sigint_cb(struct ev_loop *reactor, ev_signal *int_w, int revents)
{
    log_info("int recived");
    ev_break(reactor, EVBREAK_ONE);
    return;
}

static void sigpipe_cb(struct ev_loop *reactor, ev_signal *int_w, int revents)
{

    return;
}

void signal_init(data_t *pdata)
{
    ev_signal *int_w = (ev_signal*)talloc(sizeof(ev_signal));
    ev_init(int_w, sigint_cb);
    ev_signal_set(int_w, SIGINT);
    ev_signal_start(pdata->reactor, int_w);

    ev_signal *pipe_w = (ev_signal*)talloc(sizeof(ev_signal));
    ev_signal_init(pipe_w, sigpipe_cb, SIGPIPE);
    ev_signal_start(pdata->reactor, pipe_w);

    return;
}
