#include "display_confirmation.h"
extern void FUN_0040f8f0(uint32_t *destination,uint32_t count,uint8_t value);
void FUN_004cf1c0(void)
{
    uint32_t destination=DC_U(0xe92e18);
    if(destination){
        uint32_t count=DC_U(0xe38dac)*DC_U(0xe38db0)*2u;
        FUN_0040f8f0((uint32_t *)(uintptr_t)destination,count,0);
    }
}
