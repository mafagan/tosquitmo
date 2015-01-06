#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ev.h>

#include "net.h"
#include "config.h"
#include "net_read_handle.h"

static void accept_handle(struct ev_loop *reactor, ev_io *w, int events)
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int connfd = accept(w->fd, (struct sockaddr*)&addr, &addr_len);
    if(connfd < 0x00){
        perror("accept error");
        return;
    }

    ev_io *cw = (ev_io*)malloc(sizeof(ev_io));
    ev_io_init(cw, socket_read_handle, connfd, EV_READ);
    ev_io_start(reactor, cw);

}

int main(int argv, char **argc)
{
    config_init();

    ev_io *listensock = (ev_io*)malloc(sizeof(ev_io));
    int listenfd = new_tcp_listensock();

    struct ev_loop *reactor = EV_DEFAULT;
    ev_io_init(listensock, accept_handle, listenfd, EV_READ);
    ev_io_start(reactor, listensock);
    ev_run(reactor, 0);
    return 0;
}
