#include <pthread.h>

/* for sleep */
#include <time.h>
#include <unistd.h>

/* for strncpy */
#include <string.h>

#include "tosquitmo_broker.h"
#include "database.h"
#include "tosquitmo.h"
#include "logging.h"
#include "memory_pool.h"

extern data_t pdata;

/* return memory */
static void tos_destroy_msg(tosquitmo_message_t *msg)
{
    //TODO
}

static int _clientid_exist(char *clientid)
{
    return 0;
}

static void tos_connack_write()
{

}


static void tos_connect_handle(tosquitmo_message_t *msg)
{
    char version = (*(char*)(msg->content+8));
    char flags = (*(char*)msg->content+9);
    int username_flag = (0x01 & (flags >> 7));
    int password_flag = (0x01 & (flags >> 6));
    // int retaing_flag = (0x01 & (flags >> 5));
    // int willqos_flag = (0x03 & (flags >> 3));
    int will_flag = (0x01 & (flags >> 2));
    // int clean_session_flag = (0x01 & (flags >> 1));

    int keepalive = (((*(char*)(msg->content+10)) << 8) + (*(char*)(msg->content+11)));
    char *payload = msg->content + 12;

    if(version != 0x03){

    }

    session_t *session = msg->session;
    pthread_mutex_lock(&msg->session->session_lock);

    session->keepalive = keepalive;
    session->connect_flags = *(msg->content + 9);

    /* get client identifier */
    int len = (((*payload) << 8) + (*(payload+1))) & 0xff;
    session->identifier = (char*)talloc(len+1);
    strncpy(session->identifier, payload+2, len);
    session->identifier[len] = '\0';

    if(_clientid_exist(session->identifier)){
        //TODO
    }

    payload = payload + 2 + len;

    /* get will topic, message and will qos */
    if(will_flag){
        len = (((*payload) << 8) + (*(payload+1))) & 0xff;
        session->will_topic = (char*)talloc(len+1);
        strncpy(session->will_topic, payload + 2, len);
        session->will_topic[len] = '\0';

        payload = payload + 2 + len;
        len = (((*payload) << 8) + (*(payload+1))) & 0xff;
        session->will_message = (char*)talloc(len+1);
        strncpy(session->will_message, payload + 2, len);
        session->will_message[len] = '\0';

        payload = payload + 2 + len;
    }


    if(username_flag){
        len = (((*(char*)(payload)) << 8) + (*(char*)(payload+1))) & 0xff;
        session->username = (char*)talloc(len+1);
        strncpy(session->username, payload + 2, len);
        session->username[len] = '\0';

        payload = payload + 2 + len;
    }

    if(password_flag){
        len = (((*(char*)(payload)) << 8) + (*(char*)(payload+1))) & 0xff;
        session->password= (char*)talloc(len+1);
        strncpy(session->password, payload + 2, len);
        session->password[len] = '\0';

        payload = payload + 2 + len;
    }

    pthread_mutex_unlock(&msg->session->session_lock);
}

static void tos_publish_handle(tosquitmo_message_t *msg)
{

}

static void tos_pubrec_handle(tosquitmo_message_t *msg)
{

}

static void tos_pubrel_handle(tosquitmo_message_t *msg)
{

}

static void tos_pubcomp_handle(tosquitmo_message_t *msg)
{

}

static void tos_subscribe_handle(tosquitmo_message_t *msg)
{

}

static void tos_unsucribe_handle(tosquitmo_message_t *msg)
{

}

static void tos_pingreq_handle(tosquitmo_message_t *msg)
{

}

static void tos_pingresp_handle(tosquitmo_message_t *msg)
{

}

static void tos_disconnect_handle(tosquitmo_message_t *msg)
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
        tos_connect_handle(msg_ptr);
        break;

        case CONNACK:
        //TBD
        break;

        case PUBLISH:
        tos_publish_handle(msg_ptr);
        break;

        case PUBACK:
        //TBD
        break;

        case PUBREC:
        tos_pubrec_handle(msg_ptr);
        break;

        case PUBREL:
        tos_pubrel_handle(msg_ptr);
        break;

        case PUBCOMP:
        tos_pubcomp_handle(msg_ptr);
        break;

        case SUBSCRIBE:
        tos_subscribe_handle(msg_ptr);
        break;

        case SUBACK:
        //TBD
        break;

        case UNSUBSCRIBE:
        tos_unsucribe_handle(msg_ptr);
        break;

        case PINGREQ:
        tos_pingreq_handle(msg_ptr);
        break;

        case PINGRESP:
        tos_pingresp_handle(msg_ptr);
        break;

        case DISCONNECT:
        tos_disconnect_handle(msg_ptr);
        break;

        case RESERVEDB:
        break;

        default:
        break;

    }
    tos_destroy_msg(msg_ptr);
    return;
}
