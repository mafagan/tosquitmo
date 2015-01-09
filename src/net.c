
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "net.h"
#include "config.h"
#include "net_read_handle.h"
#include "memory_pool.h"
#include "tosquitmo.h"


void setnonblock(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void setreuseaddr(int fd)
{
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));
}

void setaddress(const char *ip, int port, struct sockaddr_in *addr)
{
    bzero(addr, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    inet_pton(AF_INET, ip, &(addr->sin_addr));
    addr->sin_port = htons(port);
}

int new_tcp_listensock(data_t *pdata)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0x00){
        perror("socket error");
        return fd;
    }

    setnonblock(fd);
    setreuseaddr(fd);
    struct sockaddr_in addr;
    setaddress("0.0.0.0", pdata->config->listenport, &addr);
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(fd, 10);
    return fd;
}


static void accept_handle(struct ev_loop *reactor, ev_io *w, int events)
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int connfd = accept(w->fd, (struct sockaddr*)&addr, &addr_len);
    if(connfd < 0x00){
        perror("accept error");
        return;
    }

    setnonblock(connfd);

    session_t *new_session = (session_t*)talloc(sizeof(session_t));

    ev_io_init(&(new_session->w), socket_read_handle, connfd, EV_READ);
    ev_io_start(reactor, &(new_session->w));

}


void net_init(data_t *pdata)
{
    pdata->listenfd = new_tcp_listensock(pdata);
    ev_io listensock;
    ev_io_init(&listensock, accept_handle, pdata->listenfd, EV_READ);
    ev_io_start(pdata->reactor, &listensock);
}
