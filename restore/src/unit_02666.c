#include "sprite_cycle_numeric.h"
uint32_t restore_sprite_cycle_add_reverse(RestoreSpriteCycle *self,uint32_t step)
{
    return cycle_sum(self,step,0,0.0f);
}
RESTORE_CYCLE_ENTRY1(FUN_0055d4c0,restore_sprite_cycle_add_reverse)
