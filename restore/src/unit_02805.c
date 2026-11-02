#include "legacy_format.h"
#include <stdio.h>
#include <stdarg.h>
int __cdecl FUN_00561a36(char *destination,const char *format,...)
{
    int result;
    va_list arguments;
    va_start(arguments,format);
    result=vsprintf(destination,format,arguments);
    va_end(arguments);
    return result;
}
