#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ev.h>
#include <pthread.h>

#include "net.h"
#include "config.h"
#include "net_read_handle.h"
#include "signal_handle.h"
#include "memory_pool.h"
#include "tosquitmo.h"
#include "thread_pool.h"

extern config_t config;

int main(int argv, char **argc)
{
    config_init();
    memory_pool_init();
    pthread_t *threads = (pthread_t*)talloc(config.thread_num*sizeof(pthread_t));
    thread_pool_init(threads, config.thread_num);

    struct ev_loop *reactor = EV_DEFAULT;
    int listenfd = new_tcp_listensock();

    net_init(reactor, listenfd);
    signal_init(reactor);
    ev_run(reactor, 0);

    thread_pool_destroy(threads, config.thread_num);
    close(listenfd);
    return 0;
}
