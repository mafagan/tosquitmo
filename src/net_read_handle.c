
#include <unistd.h>

#include "net_read_handle.h"
#include "net.h"
#include "types.h"

void socket_read_handle(struct ev_loop *reactor, ev_io *w, int events)
{
    session_t *s_session = (session_t*)w;
    char buf[256];
    if(s_session->recv_length < HEADER_LENGTH){
        int r_len = read(w->fd, buf, HEADER_LENGTH - s_session->recv_length);

        if(r_len == 0){
            //TODO
        }else{

        }
    }
    return;
}
