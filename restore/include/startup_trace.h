#ifndef RESTORE_STARTUP_TRACE_H
#define RESTORE_STARTUP_TRACE_H
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
/* Optional observations of real startup calls; never changes their results. */
static void restore_startup_trace(const char *format,...)
{
    char enabled[8],message[1024];va_list arguments;
    if(GetEnvironmentVariableA("RESTORE_TRACE_STARTUP",enabled,sizeof(enabled))!=1||enabled[0]!='1') return;
    va_start(arguments,format);vsnprintf(message,sizeof(message),format,arguments);va_end(arguments);
    OutputDebugStringA(message);
}
#endif
