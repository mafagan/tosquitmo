#include "memory_pool.h"

void* mem_alloc(size_t size)
{
    return malloc(size);
}
