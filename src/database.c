#include "database.h"
#include "memory_pool.h"
#include "types.h"

void subtree_init(subtree_node_t *root)
{
    root->topic = NULL;
    root->child = NULL;
    root->next = root;
    root->prev = root;
    root->sub_count = 0;
    root->suber_list = NULL;
    return;
}

void _sub_add_child(char *topic, subtree_node_t *node)
{

}

void _sub_add_brother(char *topic, subtree_node_t *node)
{

}
