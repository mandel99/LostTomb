#include "building_allocator.h"

/* The native entry and the construction graph now share the same complete
   typed C implementation of 46A400. No legacy scaled-pointer body remains. */
int32_t FUN_0046a400(uint32_t owner,uint32_t kind,int32_t x,int32_t y,int32_t contiguous,int32_t selected)
{
    return restore_building_allocate_0046a400(owner,kind,x,y,contiguous,selected);
}
