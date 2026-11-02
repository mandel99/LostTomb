#include "city_prepare.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
/* SETcc writes AL only in the native function; retain the remaining EAX bits. */
uint32_t FUN_0046f930(int32_t x,int32_t y)
{
    int32_t value,bound;uint32_t condition;
    if(x>I(0xbca2f8)) {
        if(y<=I(0xbca2fc)) {
            value=(int32_t)((uint32_t)x-(uint32_t)y);
            bound=(int32_t)((uint32_t)I(0xbca2f0)-(uint32_t)I(0xbca2fc));
        } else {
            value=(int32_t)((uint32_t)x+(uint32_t)y);
            bound=(int32_t)((uint32_t)I(0xbca2f0)+1u+(uint32_t)I(0xbca2fc));
        }
        condition=value<=bound;
    } else {
        if(y<=I(0xbca2fc)) {
            value=(int32_t)((uint32_t)x+(uint32_t)y);
            bound=(int32_t)((uint32_t)I(0xbca2e8)+(uint32_t)I(0xbca2fc));
        } else {
            value=(int32_t)((uint32_t)x-(uint32_t)y);
            bound=(int32_t)((uint32_t)I(0xbca2f8)-(uint32_t)I(0xbca2f4));
        }
        condition=value>=bound;
    }
    return ((uint32_t)value&0xffffff00u)|condition;
}
