#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "log.h"


#define MAX_LOG_LEN	2048
static int need_nextline = 0;
static FILE *fmsg;

LogLevel_t g_sys_loglevel = LOG_INFO;

static const char *levels[] = {
  "CRIT", "ERROR", "WARNING", "INFO",
  "DEBUG", "DEBUG2","ALL"
};

static void _log_out(LogLevel_t level, const char *format, va_list vl)
{
	char str[MAX_LOG_LEN]="";

	vsnprintf(str, MAX_LOG_LEN-1, format, vl);

	/* Filter out 'no-name' */
	//if (RTMP_debuglevel < RTMP_LOGDEBUG && strstr(str, "no-name") != NULL)
	//  return;

	if ( !fmsg ) 
        fmsg = stderr;

	if (need_nextline)
    {
		putc('\n', fmsg);
		need_nextline = 0;
	}
    
	fprintf(fmsg, "%s: %s\n", levels[level], str);
	fflush(fmsg);
}



void sys_Log(LogLevel_t level, const char *format, ...)
{
	if (level > g_sys_loglevel)
		return;
    
	va_list args;
	va_start(args, format);
	_log_out(level, format, args);
	va_end(args);
}

//
void sys_log_hex(void);
void sys_memdumphex(const void *pdata, unsigned int len)
{
    unsigned int i = 0;
    //check pdata not NULL
    printf("mem_dump\r\n");
    for(i; i<= len; i++)
    {
        
    }
}
