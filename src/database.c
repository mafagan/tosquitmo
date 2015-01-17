#include "database.h"
#include "memory_pool.h"

void db_init(data_t *pdata)
{
    pdata->sub_tree_root = (subtree_node_t*)talloc(sizeof(sub_tree_root));

    subtree_node_t *root = pdata->sub_tree_root;
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
