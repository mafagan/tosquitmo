
#include <unistd.h>
#include <string.h>
#include <errno.h>


#include "net_read_handle.h"
#include "net.h"
#include "types.h"
#include "logging.h"
#include "memory_pool.h"
#include "database.h"
#include "tosquitmo.h"

extern data_t pdata;

static void _clean_session(session_t *session)
{
    session->recv_length = 0;
    session->remaining_read = 0;
    session->to_process = HEADER;
    memset(session->remain_bytes, 0, 4);
}

static int get_remaining_length(char *length_buf)
{
    int multiplier = 1;
    int value = 0;
    char *digit= length_buf - 1;

    do{
        ++ digit;
        value += ((*digit) & 127) * multiplier;
        multiplier *= 128;

    }while((*digit & 128) !=0);

    return value;
}


/*
 * network data recive and mqtt data packet handle
 *
 **/

void socket_read_handle(struct ev_loop *reactor, ev_io *w, int events)
{
    session_t *s_session = (session_t*)w;
    if(s_session->to_process == HEADER){

        /* here is a brand new packet */
        int r_len = read(w->fd, &s_session->header, 1);

        if(r_len == 0){
            log_error("unexpected EOF");
            remove_session(s_session);
            return;
        }

        s_session->recv_length += 1;
        //s_session->command = get_command(s_session->header);
        s_session->to_process = REMAINING;
    }

    if(s_session->to_process == REMAINING){

        /*
         * get "remaining" part in header, it may takes 1~4 bytes, and it has
         * to read one by one.
         *
         */

        do{
            int r_len = read(w->fd, s_session->remain_bytes+(int)s_session->remaining_read, 1);

            if(r_len == 0){
                log_error("unexpected EOF");
                remove_session(s_session);
                return;
            }

            if(r_len < 0){
                if(errno == EAGAIN || errno == EINTR){
                    /* exit wait for next data in */
                    return;
                }else{
                    log_error("error connection");
                    remove_session(s_session);
                    return;
                }
            }

            s_session->remaining_read += 1;
            s_session->recv_length += r_len;

            /* it means at least one more byte exist */
            if((s_session->remain_bytes[s_session->remaining_read-1] & 128) > 0){

                /* 4 bytes at most for repesenting remaining length */
                if( s_session->remaining_read >= MQTT_LENGTH_MAX_BYTE){
                    log_error("protocol error");
                    remove_session(s_session);
                    return;
                }
            }else{
                /* all bytes for remaining length read */
                s_session->remaining_length = get_remaining_length(s_session->remain_bytes);
                s_session->to_process = CONTENT;
                s_session->recv_length = 0;
                s_session->content = (char*)talloc(s_session->remaining_length);
                break;
            }

        }while(1);
    }

    if(s_session->to_process == CONTENT){

        /* at least one byte left to read */

        do{
            int r_len;
            r_len = read(w->fd, s_session->content, s_session->remaining_length-s_session->recv_length);

            if(r_len == 0){
                log_error("unexpected EOF");
                remove_session(s_session);
                return;
            }

            if(r_len < 0){
                if(errno == EAGAIN || errno == EINTR){
                    return;
                }else{
                    log_error("error connection");
                    remove_session(s_session);
                    return;
                }
            }

            s_session->recv_length += r_len;

            if(s_session->recv_length >= s_session->remaining_length){
                _msg_add(pdata.msg_queue, s_session);
                _clean_session(s_session);
                break;
            }
        }while(1);
    }
    return;
}
