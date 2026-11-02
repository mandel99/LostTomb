#include "monument_constructor.h"
void *FUN_004e5120(void *object,int32_t x,int32_t y,int32_t size,int32_t type)
{
 volatile uint32_t *p=object;p[0]=(uint32_t)x;p[1]=(uint32_t)y;p[2]=(uint32_t)size;p[3]=(uint32_t)type;
 return object;
}
