#include "god_effects.h"

/* Route the address entry to the same whole typed C function used by the
   simulation. Its 30-byte message scratch retains native unspecified bytes;
   do not replace that prestate with invented zero/default assignments. */
void FUN_004c47a0(int32_t god,uint32_t source,int32_t effect)
{
    restore_god_effect_004c47a0(god,source,effect);
}
