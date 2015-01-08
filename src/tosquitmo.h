#ifndef _TOSQUITMO_H_
#define _TOSQUITMO_H_

#include <ev.h>
#include <pthread.h>

#include "config.h"
#include "net.h"

typedef struct{
    session_t *session_head;
    config_t *config;
    int listenfd;
    struct ev_loop *reactor;
    pthread_t *threads;
}data_t;



#endif
