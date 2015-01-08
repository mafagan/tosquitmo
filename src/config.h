
#ifndef _CONFIG_H
#define _CONFIG_H

typedef struct config{
    int listenport;
    short thread_num;
} config_t;

void config_init(data_t *pdata);
#endif
