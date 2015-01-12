#ifndef _LOGGING_H_
#define _LOGGING_H_

#define LOGGING_DEBUG 0x01
#define LOGGING_INFO  0x02
#define LOGGING_WARN  0x04
#define LOGGING_ERROR 0x08

void logging(const char *, int, int priority, const char*, ...);
void log_init();

#define log_debug(fmt, args...)\
    logging(__FILE__, __LINE__, LOGGING_DEBUG, fmt, ##args)

#define log_info(fmt, args...)\
    logging(__FILE__, __LINE__, LOGGING_INFO, fmt, ##args)

#define log_warn(fmt, args...)\
    logging(__FILE__, __LINE__, LOGGING_WARN, fmt, ##args)

#define log_error(fmt, args...)\
    logging(__FILE__, __LINE__, LOGGING_ERROR, fmt, ##args)

#endif
