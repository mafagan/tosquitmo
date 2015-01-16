#include "database.h"
#include "memory_pool.h"

void db_init(subtree_t *tree)
{
    tree->root = (subtree_node_t*)talloc(sizeof(subtree_node_t));
    return;
}
