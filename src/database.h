#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "types.h"
#include "tosquitmo.h"
#include "uthash.h"

struct suber_node{
    session_t *session;
    int qos;
    struct suber_node *next;
};

typedef struct subtree_node{
    char *topic;    /* also the key for hash table*/
    int sub_count;
    struct suber_node *suber_list;
    struct suber_node *tail_node;
    struct subtree_node *children;
    UT_hash_handle hh;
}subtree_node_t;

void subtree_init(data_t *pdata);
void message_queue_init(data_t *pdata);
void _msg_add(tosquitmo_message_queue_t *msg_queue, session_t *session);
void _sub_add_brother(char *topic, subtree_node_t *node);
void _sub_add_child(char *topic, subtree_node_t *node);
#endif
