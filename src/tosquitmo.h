#ifndef _TOSQUITMO_H_
#define _TOSQUITMO_H_

#include <ev.h>
#include <pthread.h>

#include "types.h"
#include "uthash.h"

#define CONN_ACCEPT 0x00
#define CONN_REFUSE_VERSION 0x01
#define CONN_REFUSE_IDENTIFIER 0x02
#define CONN_REFUSE_SERVER_UNAVAILABLE 0x03
#define CONN_REFUSE_BAD_USR_OR_PWD 0x04
#define CONN_REFUSE_NOT_AUTHORIZED 0x05

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


struct client_id_struct{
    char identifier[24];
    UT_hash_handle hh;
};
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
    struct client_id_struct *id_table;
    pthread_mutex_t ctrl_flag_lock;
}data_t;


void _msg_destroy(tosquitmo_message_t *msg);
#endif
