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
#define CONTENT 2

#define MQTT_LENGTH_MAX_BYTE 4

#define MQTT_CONNECT_VAR_LEN 12
#define MQTT_CONNACK_VAR_LEN 2
#define MQTT_PUBACK_VAR_LEN 2
#define MQTT_PUBREC_VAR_LEN 2
#define MQTT_PUBREL_VAR_LEN 2
#define MQTT_PUBCOMP_VAR_LEN 2
#define MQTT_SUBSCRIBE_VAR_LEN 2
#define MQTT_SUBACK_VAR_LEN 2
#define MQTT_UNSUBSCRIBE_VAR_LEN 2
#define MQTT_UNSUBACK_VAR_LEN 2


typedef struct session{
    ev_io w;
    char *identifier;
    char remain_bytes[4];
    char remaining_read;
    char header;
    char to_process;
    int command;
    int recv_length;
    int remaining_length;
    int keepalive;
    char *content;
    char *username;
    char *password;
    pthread_mutex_t session_lock;
    struct session *next_session;
}session_t;


typedef struct config{
    int listenport;
    short thread_num;
} config_t;


#endif
