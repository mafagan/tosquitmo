#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "tosquitmo_broker.h"
#include "database.h"
#include "tosquitmo.h"

extern data_t pdata;
static void tos_destroy_msg(tosquitmo_message_t *msg)
{

}

static void tos_connect(tosquitmo_message_t *msg)
{

}

static void tos_publish(tosquitmo_message_t *msg)
{

}

static void tos_pubrec(tosquitmo_message_t *msg)
{

}

static void tos_pubrel(tosquitmo_message_t *msg)
{

}

static void tos_pubcomp(tosquitmo_message_t *msg)
{

}

static void tos_subscribe(tosquitmo_message_t *msg)
{

}

static void tos_unsucribe(tosquitmo_message_t *msg)
{

}

static void tos_pingreq(tosquitmo_message_t *msg)
{

}

static void tos_pingresp(tosquitmo_message_t *msg)
{

}

static void tos_disconnect(tosquitmo_message_t *msg)
{

}


void tos_exec_cmd(tosquitmo_message_queue_t *msg_queue)
{
    pthread_mutex_lock(&msg_queue->head_lock);
    pthread_mutex_lock(&msg_queue->end_lock);

    if(msg_queue->end == NULL){
        pthread_mutex_unlock(&msg_queue->head_lock);
        pthread_mutex_unlock(&msg_queue->end_lock);

        /* reduce cpu load */
        usleep(1);
        return;
    }

    tosquitmo_message_t *msg_ptr = msg_queue->head;

    /* if there is only one message in queue, end_ptr need to
     * be modified */

    if(msg_queue->end == msg_queue->head){
        msg_queue->end = msg_queue->head->next;
    }
    msg_queue->head = msg_queue->head->next;

    pthread_mutex_unlock(&msg_queue->head_lock);
    pthread_mutex_unlock(&msg_queue->end_lock);


    /*decode msg packet */

    /* message stored in byte 4~7 */
    char msg_type = 0x0f & (msg_ptr->header >> 4);
    switch(msg_type){
        case RESERVEDE:
        break;

        case CONNECT:
        tos_connect(msg_ptr);
        break;

        case CONNACK:
        //TBD
        break;

        case PUBLISH:
        tos_publish(msg_ptr);
        break;

        case PUBACK:
        //TBD
        break;

        case PUBREC:
        tos_pubrec(msg_ptr);
        break;

        case PUBREL:
        tos_pubrel(msg_ptr);
        break;

        case PUBCOMP:
        tos_pubcomp(msg_ptr);
        break;

        case SUBSCRIBE:
        tos_subscribe(msg_ptr);
        break;

        case SUBACK:
        //TBD
        break;

        case UNSUBSCRIBE:
        tos_unsucribe(msg_ptr);
        break;

        case PINGREQ:
        tos_pingreq(msg_ptr);
        break;

        case PINGRESP:
        tos_pingresp(msg_ptr);
        break;

        case DISCONNECT:
        tos_disconnect(msg_ptr);
        break;

        case RESERVEDB:
        break;

        default:
        break;

    }
    tos_destroy_msg(msg_ptr);
    return;
}
