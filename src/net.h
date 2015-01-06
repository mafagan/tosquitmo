#ifndef _NET_H_
#define _NET_H_

#include <netinet/in.h>

void setnonblock(int fd);
void setreuseaddr(int fd);
void setaddress(const char *ip, int port, struct sockaddr_in *addr);
int new_tcp_listensock();

#endif
