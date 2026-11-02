#include "map_menu_scroll.h"
/* Original 00442800: signed comparisons, modular DWORD bound arithmetic.
   Preserve lower-then-upper clamp even when a nonpositive bound leaves -1. */
void FUN_00442800(void)
{
    uint32_t position,padding,screen,extent,other,bound;
    MSE();
    position=MSR(0x775910u);
    if((int32_t)position<0){position=0;MSW(0x775910u,0);}
    padding=MSR(0x5d2a18u);screen=MSR(0xe38d9cu);extent=MSR(0x5d2a10u);
    bound=extent+(padding+padding-screen);
    if((int32_t)position>=(int32_t)bound)MSW(0x775910u,bound-1u);
    position=MSR(0x77590cu);
    if((int32_t)position<0){position=0;MSW(0x77590cu,0);}
    padding=MSR(0x5d2a20u);screen=MSR(0xe38d98u);other=MSR(0x5d2a1cu);extent=MSR(0x5d2a14u);
    bound=extent+(padding-screen+other);
    if((int32_t)position>=(int32_t)bound)MSW(0x77590cu,bound-1u);
}
