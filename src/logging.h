#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>

#define LOGGING_DEBUG 0x01
#define LOGGING_INFO  0x02
#define LOGGING_WARN  0x04
#define LOGGING_ERROR 0x08

#define DATE_LEN  26

void logging(int priority, const char*, ...);
void log_init();

#define log_debug(fmt, args...)\
    logging(LOGGING_DEBUG, fmt, ##args)

#define log_info(fmt, args...)\
    logging(LOGGING_INFO, fmt, ##args)

#define log_warn(fmt, args...)\
    logging(LOGGING_WARN, fmt, ##args)

#define log_error(fmt, args...)\
    logging(LOGGING_ERROR, fmt, ##args)

void log_init()
{

    return;
}

void logging(int priority, const char *fmt, ...)
{
    va_list args;
    char buf[1024];
    time_t t;
    int curlength = 0;

    t = time(NULL);
    struct tm *local_time = localtime(&t);
    strftime(buf+curlength, DATE_LEN+1, "[%Y-%m-%d %H:%M:%S] ", local_time);
    curlength += (DATE_LEN+1);

    int trans_len;
    if(priority & LOGGING_INFO){
        trans_len = sprintf(buf+curlength, "[INFO]: ");
    }else if(priority & LOGGING_DEBUG){
        trans_len = sprintf(buf+curlength, "[DEBUG]: ");
    }else if(priority & LOGGING_WARN){
        trans_len = sprintf(buf+curlength, "[WARN]: ");
    }else{
        trans_len = sprintf(buf+curlength, "[ERROR]: ");
    }

    assert(trans_len>0);
    curlength += trans_len;

    va_start(args, fmt);
    int res = vsnprintf(buf+curlength, sizeof(buf)-curlength-1, fmt, args);
    va_end(args);
    assert(res>=0);
    curlength += res;

    buf[curlength] = '\0';

    if(priority & LOGGING_ERROR){
        fprintf(stderr, "%s\n", buf);
    }else{
        fprintf(stdout, "%s\n", buf);
    }
    return;
}

#endif
