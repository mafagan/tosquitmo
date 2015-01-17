#ifndef _TOSQUITMO_H_
#define _TOSQUITMO_H_

#include <ev.h>
#include <pthread.h>

#include "types.h"
#include "database.h"

typedef struct{
    subtree_node_t *sub_tree_root;
    session_t *session_head;
    config_t *config;
    int listenfd;
    struct ev_loop *reactor;
    pthread_t *threads;
    session_t *session_end;
}data_t;


#endif
