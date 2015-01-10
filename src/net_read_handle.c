
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "net_read_handle.h"
#include "net.h"
#include "types.h"

static int get_remaining_length(char *length_buf)
{
    //TODO
    return 0;
}

static char get_command(char byte)
{
    //TODO
    return 0;
}
void socket_read_handle(struct ev_loop *reactor, ev_io *w, int events)
{
    session_t *s_session = (session_t*)w;
    char buf[256];
    if(s_session->recv_length == 0){
        int r_len = read(w->fd, &s_session->header, 1);

        if(r_len == 0){
            //log error EOF
            remove_session(s_session);
            return;
        }

        s_session->recv_length += 1;
        s_session->command = get_command(s_session->header);
        s_session->to_process = REMAINING;
    }

    if(s_session->to_process == REMAINING){
        do{
            int r_len = read(w->fd, &s_session->remain_bytes[(int)s_session->remaining_read], 1);

            if(r_len == 0){
                //log error EOF
                remove_session(s_session);
                return;
            }

            if(r_len < 0){
                if(errno == EAGAIN || errno == EINTR){
                    return;
                }else{
                    //log error connect
                    remove_session(s_session);
                    return;
                }
            }

            s_session->remaining_read += 1;
            if((s_session->remain_bytes[s_session->remaining_read-1] & 128) > 0){
                if( s_session->remaining_read >= MQTT_LENGTH_MAX_BYTE){
                    //log error protocol
                    remove_session(s_session);
                    return;
                }
            }else{
                s_session->remaining_length = get_remaining_length(s_session->remain_bytes);
                // change to_process
                break;
            }

        }while(1);
    }

    if(s_session->to_process == VARIABLE){

    }

    if(s_session->to_process == PAYLOAD){
        do{
            int r_len = read(w->fd, buf, sizeof(buf));

            if(r_len == 0){

            }

            if(r_len < 0){

            }
        }while(1);
    }
    return;
}
