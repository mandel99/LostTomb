#define _CRT_SECURE_NO_WARNINGS
#include "pharaoh/runtime/format_unbounded.h"

#include <stdarg.h>
#include <stdio.h>

int pharaoh_format_unbounded(char *destination, const char *format, ...)
{
    int result;
    va_list arguments;

    va_start(arguments, format);
    result = vsprintf(destination, format, arguments);
    va_end(arguments);
    return result;
}
