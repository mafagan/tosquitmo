#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

#include <stdlib.h>

#include "types.h"
#include "tosquitmo.h"

void memory_pool_init(data_t *pdata);
void* talloc(size_t size);
void tfree(void *ptr);
#endif
