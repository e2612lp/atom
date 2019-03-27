#ifndef _LOG_H
#define _LOG_H

typedef enum
{ 
    LOG_CRIT=0, LOG_ERROR, LOG_WARNING, LOG_INFO,
    LOG_DEBUG, LOG_DEBUG2, LOG_ALL
}LogLevel_t;

extern LogLevel_t g_sys_loglevel;
void sys_Log(LogLevel_t level, const char *format, ...);

#endif
