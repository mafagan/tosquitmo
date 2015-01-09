#ifndef _TYPES_H_
#define _TYPES_H_

#include <ev.h>
#include <pthread.h>

typedef struct{
}subtree_t;


typedef struct session{
    ev_io w;
    struct session *next_session;
}session_t;


typedef struct config{
    int listenport;
    short thread_num;
} config_t;



typedef struct{
    session_t *session_head;
    config_t *config;
    int listenfd;
    struct ev_loop *reactor;
    pthread_t *threads;
}data_t;



#endif
