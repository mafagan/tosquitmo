#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

#include <stdlib.h>


void memory_pool_init();
void* talloc(size_t size);

#endif
