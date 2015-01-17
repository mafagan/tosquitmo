#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "types.h"

typedef struct subtree_node{
    char *topic;
    subtree_node_t *child;
    subtree_node_t *next;
    subtree_node_t *prev;
    int sub_count;
    session_t *suber_list;
}subtree_node_t;

void db_init(data_t *pdata);
void _sub_add_brother(char *topic, subtree_node_t *node);
void _sub_add_child(char *topic, subtree_node_t *node);
#endif
