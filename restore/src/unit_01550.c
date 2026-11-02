#include "city_prepare.h"
void *FUN_004bd610(void *item,int32_t index,int32_t x,int32_t y,int32_t kind,RestoreWorldRegion *owner,uint8_t flag)
{
    uint32_t *p=(uint32_t *)item;
    p[2]=(uint32_t)index;p[3]=(uint32_t)x;p[4]=(uint32_t)y;p[5]=(uint32_t)kind;
    p[1]=0;p[0]=(uint32_t)(uintptr_t)owner;((uint8_t *)item)[24]=flag;return item;
}
