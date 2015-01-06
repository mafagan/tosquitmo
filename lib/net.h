#ifndef _NET_H_
#define _NET_H_

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void setnonblock(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

static void setreuseaddr(int fd)
{
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));
}

static void setaddress(const char *ip, int port, struct sockaddr_in *addr)
{
    bzero(addr, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    inet_pton(AF_INET, ip, &(addr->sin_addr));
    addr->sin_port = htons(port);
}

static int new_tcp_listensock(int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0x00){
        perror("socket error");
        return fd;
    }

    setnonblock(fd);
    setreuseaddr(fd);
    struct sockaddr_in addr;
    setaddress("0.0.0.0", port, &addr);
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(fd, 10);
    return fd;
}


#endif
