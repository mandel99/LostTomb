#include "sprite_cycle_numeric.h"
uint32_t restore_sprite_cycle_add_forward(RestoreSpriteCycle *self,uint32_t step)
{
    return !cycle_sum(self,step,0,cycle_length(self));
}
RESTORE_CYCLE_ENTRY1(FUN_0055d480,restore_sprite_cycle_add_forward)
