#include "byte_fill.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Native40F8F0 writes complete DWORDs, then trailing BYTEs. Preserve widths,
   forward order, wrapped addresses and partial writes before a fault. */
__declspec(noinline) void restore_memory_fill_bytes(uint32_t *destination,uint32_t bytes,uint8_t value)
{
    uint32_t base=(uint32_t)(uintptr_t)destination,words=bytes>>2,i,pattern=(uint32_t)value*0x01010101u;
    for(i=0;i<words;++i)U(base+i*4u)=pattern;
    base+=words*4u;
    for(i=0;i<(bytes&3u);++i)B(base+i)=value;
}
__declspec(noinline) void FUN_0040f8f0(uint32_t *destination,uint32_t bytes,uint8_t value)
{restore_memory_fill_bytes(destination,bytes,value);}
