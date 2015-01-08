
#include <stdlib.h>

#include "tosquitmo.h"
#include "config.h"

void config_init(data_t *pdata)
{
    //TODO
    pdata->config = (config_t*)malloc(sizeof(config_t));
    pdata->config->listenport = 1688;
    pdata->config->thread_num = 2;
}
