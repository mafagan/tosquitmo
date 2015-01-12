
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>


#include "logging.h"

void log_init()
{

    return;
}

void logging(const char *logfile, int logline, int priority, const char *fmt, ...)
{
    va_list args;
    char buf[1024];
    time_t t;
    int curlength = 0;

    t = time(NULL);
    struct tm *local_time = localtime(&t);
    int time_len = strftime(buf+curlength, 256, "[%Y-%m-%d %H:%M:%S]", local_time);
    curlength += time_len;

    int filelen;
    if(priority & (LOGGING_ERROR |LOGGING_WARN)){
        filelen = sprintf(buf+curlength, " %s(%d)", logfile, logline);
        assert(filelen>0);
        curlength += filelen;
    }

    int trans_len;
    if(priority & LOGGING_INFO){
        trans_len = sprintf(buf+curlength, " INFO: ");
    }else if(priority & LOGGING_DEBUG){
        trans_len = sprintf(buf+curlength, " DEBUG: ");
    }else if(priority & LOGGING_WARN){
        trans_len = sprintf(buf+curlength, " WARN: ");
    }else{
        trans_len = sprintf(buf+curlength, " ERROR: ");
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
