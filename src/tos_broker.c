#include <pthread.h>

/* for sleep */
#include <time.h>
#include <unistd.h>

/* for strncpy */
#include <string.h>

/* for iov */
#include <sys/uio.h>

/* for slink-list */
#include <sys/queue.h>

#include "tos_broker.h"
#include "database.h"
#include "tosquitmo.h"
#include "logging.h"
#include "memory_pool.h"
#include "uthash.h"


extern data_t pdata;

typedef struct tos_topic_token{
    char *token;
    struct tos_topic_token *next;
}tos_topic_token_t;

static char* _set_backslash_null(char *begin_ptr)
{
    return NULL;
}

static int _find_topic_level_end(char *str)
{

    return 0;
}


static int is_usrname_password_valid(char *username, char *password)
{

    return 0;
}

static void tos_suback_write(char *qosArray, int qos_len, char *msg_id, session_t *session)
{
    char header = 0xff & (9 << 4);
    struct iovec iov[4];
    iov[0].iov_base = &header;
    iov[0].iov_len = 1;

    /* TODO remaining length */
    iov[2].iov_base = msg_id;
    iov[2].iov_len = 2;

    iov[3].iov_base = qosArray;
    iov[3].iov_len = qos_len;
    writev(session->w.fd, iov, 4);

}

static void tos_connack_write(int ret, session_t *session)
{
    char *content = (char*)talloc(4);

    *content = 0x20;
    *(content+1) = 0x02;
    *(content+3) = (char)ret;

    write(session->w.fd, content, 4);
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

    session_t *session = msg->session;
    pthread_mutex_lock(&msg->session->session_lock);


    if(version != 0x03){
        tos_connack_write(CONN_REFUSE_VERSION, session);
        pthread_mutex_unlock(&session->session_lock);
        return;
    }
    session->keepalive = keepalive;
    session->connect_flags = *(msg->content + 9);

    /* get client identifier */
    int len = (((*payload) << 8) + (*(payload+1))) & 0xff;
    if(len > 23){
        tos_connack_write(CONN_REFUSE_IDENTIFIER, session);
        pthread_mutex_unlock(&session->session_lock);
        return;
    }

    /* check client id unique */
    struct client_id_struct *tmp = (struct client_id_struct*)talloc(sizeof(struct client_id_struct));
    strncpy(tmp->identifier, payload+2, sizeof(tmp->identifier));

    struct client_id_struct *check_tmp;
    HASH_FIND_STR(pdata.id_table, tmp->identifier, check_tmp);

    if(check_tmp){
        tos_connack_write(CONN_REFUSE_IDENTIFIER, session);
        pthread_mutex_unlock(&session->session_lock);
        return;
    }else{
        HASH_ADD_STR(pdata.id_table, identifier, tmp);
        session->id_struct = tmp;
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

    if(!is_usrname_password_valid(session->username, session->password)){
        tos_connack_write(CONN_REFUSE_BAD_USR_OR_PWD, session);
        pthread_mutex_unlock(&session->session_lock);
        return;
    }

    tos_connack_write(CONN_ACCEPT, session);
    session->is_connected = 1;
    pthread_mutex_unlock(&msg->session->session_lock);

}

static void tos_subscribe_handle(tosquitmo_message_t *msg)
{
    //char *cur = msg->content;
    //int dup = ((msg->header) >> 3) & 0x01;
    int qos = ((msg->header) >> 1) & 0x03;
    int msg_length = msg->content_length;
    int byte_readed = 0;
    char granted_qos[1024];
    int topic_count = 0;
    char *var = msg->content;

    //TODO get message id
    char *msg_id = (char*)talloc(2);
    strncpy(msg_id, var, 2);

    if(qos >= 1)
    {
        var += 2;
        byte_readed += 2;
    }

    while(byte_readed < msg_length){

        int len = (((*var) << 8) + (*(var+1))) & 0xffff;

        if(len > 64 * 1024){
            //TODO refuse it
        }

        char *topic = var + 2;

        char *qos_ptr = topic + len;
        int topic_sub_qos = (*qos_ptr) & 0xff;
        int begin_ptr = 0, end_ptr;

        pthread_mutex_lock(&pdata.sub_tree_lock);

        subtree_node_t *cur_node = pdata.sub_tree_root;
        subtree_node_t *tmp;

        while(begin_ptr < len)
        {

            if(topic_count > TOS_MAX_TOPIC_IN_SUB){
                /* ignore the rest */
                break;
            }

            /* topic level will be seperated in  [~) mode]*/
            end_ptr = _find_topic_level_end(var+begin_ptr);

            char *cur_level = (char*)talloc(end_ptr - begin_ptr + 1);

            strncpy(cur_level, topic, end_ptr - begin_ptr);
            cur_level[end_ptr-begin_ptr] = '\0';

            HASH_FIND_STR(cur_node->children, cur_level, tmp);

            if(tmp){

                int hashtag_flag = strcmp("#", cur_level);

                if(hashtag_flag == 0x00){
                    if(end_ptr < len){
                        /* TODO reject sub */
                    }
                    tmp->tail_node->next = (struct suber_node*)talloc(sizeof(struct suber_node));
                    tmp->tail_node = tmp->tail_node->next;
                    tmp->tail_node->session = msg->session;
                    tmp->tail_node->qos = topic_sub_qos;
                    tmp->tail_node->next = NULL;
                    tmp->sub_count ++;
                }

                cur_node = tmp;
            }else{
                tmp = (subtree_node_t*)talloc(sizeof(subtree_node_t));
                tmp->topic = cur_level;
                tmp->sub_count = 1;
                tmp->children = NULL;

                if(end_ptr >= len){
                    tmp->suber_list = (struct suber_node*)talloc(sizeof(struct suber_node));
                    tmp->tail_node = tmp->suber_list;
                    tmp->tail_node->session = msg->session;
                    tmp->tail_node->qos = topic_sub_qos;
                    tmp->tail_node->next = NULL;
                }else{
                    tmp->suber_list = NULL;
                    tmp->tail_node = NULL;
                }

                HASH_ADD_KEYPTR(hh, cur_node->children, tmp->topic, strlen(tmp->topic), tmp);

                cur_node = tmp;
            }

            begin_ptr = end_ptr;
        }

        pthread_mutex_unlock(&pdata.sub_tree_lock);

        granted_qos[topic_count] = topic_sub_qos;
        topic_count += 1;
        var = var + 2 + len;
        byte_readed += 2 + len;
    }
    tos_suback_write(granted_qos, topic_count, msg_id, msg->session);

}

static void tos_publish_handle(tosquitmo_message_t *msg)
{
    //char header = msg->header;

    //int pub_qos = (header >> 1) & 0x03;
    //int dup = (header >> 3) & 0x01;

    char *var = msg->content;

    subtree_node_t *sub_node = pdata.sub_tree_root;

    if(!sub_node){
        /* empty topic tree means nothing to do */
        TOS_msg_free(msg);
        return;
    }

    int len = (((*var) << 8) + (*(var+1))) & 0xff;


    char *topic = (char*)talloc(len+1);
    char *topic_end = topic + len + 1;
    strncpy(topic, var, len);
    topic[len] = '\0';

    char *token_begin = topic;
    char *token_end = _set_backslash_null(token_begin);


    SLIST_HEAD(list_head_t, subtree_node) sub_node_head;
    SLIST_INIT(&sub_node_head);
    SLIST_INSERT_HEAD(&sub_node_head, sub_node, entry);


    SLIST_HEAD(list_tmp_head_t, subtree_node) sub_node_tmp_list;
    SLIST_INIT(&sub_node_tmp_list);

    int last_token_flag = 0;

    while(token_begin < topic_end)
    {
        /* this has to called first, because it will change '/'
         * into '\0' */
        token_end = _set_backslash_null(token_begin);

        /* check if this is the last token */
        if(token_end >= topic_end)
        {
            last_token_flag = 1;
        }else
        {
            last_token_flag = 0;
        }

        subtree_node_t *cur_sub_node, *tmp_sub_node;


        /* breadth first search */
        while(!SLIST_EMPTY(&sub_node_head))
        {

            cur_sub_node = SLIST_FIRST(&sub_node_head);
            HASH_FIND_STR(cur_sub_node, token_begin, tmp_sub_node);

            if(tmp_sub_node){
                if(last_token_flag)
                {
                    //TODO write msg
                }else
                {
                    SLIST_INSERT_HEAD(&sub_node_tmp_list, tmp_sub_node, entry);
                }
            }

            HASH_FIND_STR(cur_sub_node, "#", tmp_sub_node);

            if(tmp_sub_node)
            {
                //TODO write message to suber
            }

            HASH_FIND_STR(cur_sub_node, "+", tmp_sub_node);

            if(tmp_sub_node)
            {
                if(last_token_flag)
                {
                    //TODO write msg
                }else
                {
                    SLIST_INSERT_HEAD(&sub_node_head, tmp_sub_node, entry);

                }
            }

            SLIST_REMOVE(&sub_node_head, cur_sub_node, subtree_node, entry);
        }

        while(!SLIST_EMPTY(&sub_node_tmp_list))
        {
            tmp_sub_node = SLIST_FIRST(&sub_node_tmp_list);
            SLIST_INSERT_HEAD(&sub_node_head, tmp_sub_node, entry);
            SLIST_REMOVE(&sub_node_tmp_list, tmp_sub_node, subtree_node, entry);
        }

        token_begin = token_end;
    }
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
    TOS_msg_free(msg_ptr);
    return;
}
