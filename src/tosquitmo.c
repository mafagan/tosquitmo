#include <ev.h>
#include "net.h"
#include "config.h"

int main(int argv, char **argc)
{
    config_init();

    ev_io listensock;
    int listenfd = new_tcp_listensock();
    listensock.fd = listenfd;

    struct ev_loop *reactor = EV_DEFAULT;
    ev_io_start(reactor, &listensock);
    ev_run(reactor, 0);
    return 0;
}
