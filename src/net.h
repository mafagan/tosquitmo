#ifndef _NET_H_
#define _NET_H_

#include <netinet/in.h>
#include <ev.h>

#include "tosquitmo.h"
#include "types.h"

void setnonblock(int fd);
void setreuseaddr(int fd);
void setaddress(const char *ip, int port, struct sockaddr_in *addr);
int new_tcp_listensock(data_t *pdata);
void net_init(data_t *pdata);

#endif
