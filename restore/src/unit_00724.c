#include "earthquake_event.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* Whole44BD20: cell offset and terrain are DWORDs; all address arithmetic
   wraps in32 bits. The native predicate defines the entire EAX result. */
uint32_t restore_earthquake_tile_allowed(int32_t x,int32_t y)
{
    uint32_t cell=U(0xbca2d8u)+(uint32_t)x+(uint32_t)y*228u;
    return (U(0xaf598cu+cell*4u)&0x206u)==0u;
}
uint32_t FUN_0044bd20(int32_t x,int32_t y)
{return restore_earthquake_tile_allowed(x,y);}
