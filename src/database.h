#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "types.h"

typedef struct{

}subtree_node_t;

typedef struct{
    subtree_node_t *root;
}subtree_t;

void db_init(subtree_t *tree);

#endif
