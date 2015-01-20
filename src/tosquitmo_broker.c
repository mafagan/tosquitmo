#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "tosquitmo_broker.h"
#include "database.h"
#include "tosquitmo.h"

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
        break;

        case CONNACK:
        break;

        case PUBLISH:
        break;

        case PUBACK:
        break;

        case PUBREC:
        break;

        case PUBREL:
        break;

        case PUBCOMP:
        break;

        case SUBSCRIBE:
        break;

        case SUBACK:
        break;

        case UNSUBSCRIBE:
        break;

        case PINGREQ:
        break;

        case PINGRESP:
        break;

        case DISCONNECT:
        break;

        case RESERVEDB:
        break;

        default:
        break;

    }
    return;
}
