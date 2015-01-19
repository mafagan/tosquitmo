#include "database.h"
#include "memory_pool.h"
#include "types.h"
#include "tosquitmo.h"

void subtree_init(data_t *pdata)
{
    pdata->sub_tree_root = (subtree_node_t*)talloc(sizeof(subtree_node_t));
    pdata->sub_tree_root->topic = NULL;
    pdata->sub_tree_root->child = NULL;
    pdata->sub_tree_root->next = pdata->sub_tree_root;
    pdata->sub_tree_root->prev = pdata->sub_tree_root;
    pdata->sub_tree_root->sub_count = 0;
    return;
}

void message_queue_init(data_t *pdata)
{
    pdata->msg_queue = (tosquitmo_message_queue_t*)talloc(sizeof(tosquitmo_message_queue_t));
    pdata->msg_queue->head = pdata->msg_queue->end = NULL;
    pthread_mutex_init(&pdata->msg_queue->head_lock, NULL);
    pthread_mutex_init(&pdata->msg_queue->end_lock, NULL);
}

void _msg_add(tosquitmo_message_queue_t *msg_queue, session_t *session)
{
    tosquitmo_message_t *msg = (tosquitmo_message_t*)talloc(sizeof(tosquitmo_message_t));
    msg->header = session->header;
    msg->content = session->content;
    msg->content_length = session->remaining_length;
    msg->session = session;
    msg->next = NULL;

    pthread_mutex_lock(&msg_queue->end_lock);
    pthread_mutex_lock(&msg_queue->head_lock);

    if(msg_queue->end == NULL){
        msg_queue->head = msg;
        msg_queue->end = msg_queue->head;
    }else{
        msg_queue->end->next = msg;
        msg_queue->end = msg_queue->end->next;
    }

    pthread_mutex_unlock(&msg_queue->head_lock);
    pthread_mutex_unlock(&msg_queue->end_lock);
}

void _sub_add_child(char *topic, subtree_node_t *node)
{

}

void _sub_add_brother(char *topic, subtree_node_t *node)
{

}
