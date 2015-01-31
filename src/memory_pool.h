#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

#include <stdlib.h>

#include "types.h"
#include "tosquitmo.h"

void memory_pool_init(data_t *pdata);
void* talloc(size_t size);
void tfree(void *ptr);


#define TOS_msg_free(msg_ptr) \
    do{ \
        tfree(msg_ptr->content); \
        tfree(msg_ptr); \
    }while(0);

#endif
