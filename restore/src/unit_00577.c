#include "minimap.h"
int32_t FUN_0043e770(int32_t kind)
{
    int32_t value;
    if(kind<0||kind>=237)return -1;
    value=((const int32_t *)(uintptr_t)0x5ce008u)[kind];
    return value>0?value-1:-1;
}
