#ifndef _NET_READ_HANDLE_
#define _NET_READ_HANDLE_

#include <ev.h>
void socket_read_handle(struct ev_loop *reactor, ev_io *w, int events);
#endif
