#ifndef _TYPES_H_
#define _TYPES_H_

#include <ev.h>
#include <pthread.h>

#define RESERVEDB   0x0000
#define CONNECT     0x0001
#define CONNACK     0x0002
#define PUBLISH     0x0004
#define PUBACK      0x0008
#define PUBREC      0x0010
#define PUBREL      0x0020
#define PUBCOMP     0x0040
#define SUBSCRIBE   0x0080
#define SUBACK      0x0100
#define UNSUBSCRIBE 0x0200
#define UNSUBACK    0x0400
#define PINGREQ     0x0800
#define PINGRESP    0x1000
#define DISCONNECT  0x2000
#define RESERVEDE   0x4000

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
    char *content;
    char *username;
    char *password;
    struct session *next_session;
}session_t;


typedef struct config{
    int listenport;
    short thread_num;
} config_t;




#endif
