#ifndef _TYPES_H_
#define _TYPES_H_

#include <ev.h>
#include <pthread.h>

#define RESERVEDB   0
#define CONNECT     1
#define CONNACK     2
#define PUBLISH     3
#define PUBACK      4
#define PUBREC      5
#define PUBREL      6
#define PUBCOMP     7
#define SUBSCRIBE   8
#define SUBACK      9
#define UNSUBSCRIBE 10
#define UNSUBACK    11
#define PINGREQ     12
#define PINGRESP    13
#define DISCONNECT  14
#define RESERVEDE   15

#define HEADER  0
#define REMAINING 1
#define VARIABLE 2
#define PAYLOAD 3

#define MQTT_LENGTH_MAX_BYTE 4

typedef struct{
}subtree_t;


typedef struct session{
    ev_io w;
    char *identifier;
    char remain_bytes[4];
    char remaining_read;
    char header;
    char command;
    char to_process;
    int recv_length;
    int remaining_length;
    char *variable;
    char *payload;
    struct session *next_session;
}session_t;


typedef struct config{
    int listenport;
    short thread_num;
} config_t;



typedef struct{
    session_t *session_head;
    config_t *config;
    int listenfd;
    struct ev_loop *reactor;
    pthread_t *threads;
    session_t *session_end;
}data_t;



#endif