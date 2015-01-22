
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

extern data_t pdata;

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

    session_t *n_session = new_session();

    ev_io_init(&(n_session->w), socket_read_handle, connfd, EV_READ);
    ev_io_start(reactor, &(n_session->w));

}

session_t* new_session()
{
    session_t *n_session = (session_t*)talloc(sizeof(session_t));
    n_session->next_session = NULL;

    if(pdata.session_head != NULL){
        pdata.session_end->next_session = n_session;
        pdata.session_end = pdata.session_end->next_session;
    }else{
        pdata.session_head = n_session;
        pdata.session_end = n_session;
    }
    n_session->recv_length = 0;
    n_session->to_process = HEADER;
    n_session->remaining_read = 0;
    n_session->keepalive = 0;
    n_session->content = NULL;
    n_session->username = NULL;
    n_session->password = NULL;
    pthread_mutex_init(&n_session->session_lock, NULL);
    return n_session;
}

void remove_session(session_t *session)
{

    return;
}

void net_init(data_t *pdata)
{
    pdata->listenfd = new_tcp_listensock(pdata);
    session_t *t_session = new_session();
    ev_io_init(&t_session->w, accept_handle, pdata->listenfd, EV_READ);
    ev_io_start(pdata->reactor, &t_session->w);
}
