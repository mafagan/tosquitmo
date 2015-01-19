#ifndef _TOSQUITMO_H_
#define _TOSQUITMO_H_

#include <ev.h>
#include <pthread.h>

#include "types.h"

struct subtree_node;

typedef struct tosquitmo_message{
    char header;
    char *content;
    int content_length;
    struct tosquitmo_message *next;
    session_t *session;
}tosquitmo_message_t;

typedef struct tosquitmo_message_queue{
    tosquitmo_message_t *head;
    tosquitmo_message_t *end;
    pthread_mutex_t head_lock;
    pthread_mutex_t end_lock;
}tosquitmo_message_queue_t;


typedef struct{
    struct subtree_node *sub_tree_root;
    session_t *session_head;
    config_t *config;
    int listenfd;
    struct ev_loop *reactor;
    pthread_t *threads;
    session_t *session_end;
    tosquitmo_message_queue_t *msg_queue;
    int control_flag;
    pthread_mutex_t ctrl_flag_lock;
}data_t;


#endif
