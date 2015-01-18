#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "types.h"
#include "tosquitmo.h"

typedef struct subtree_node{
    char *topic;
    struct subtree_node *child;
    struct subtree_node *next;
    struct subtree_node *prev;
    int sub_count;
    session_t *suber_list;
}subtree_node_t;

void subtree_init(data_t *pdata);
void message_queue_init(data_t *pdata);
void _msg_add(tosquitmo_message_queue_t *msg_queue, session_t *session);
void _sub_add_brother(char *topic, subtree_node_t *node);
void _sub_add_child(char *topic, subtree_node_t *node);
#endif
