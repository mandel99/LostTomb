#include "sprite_cycle_numeric.h"
void restore_sprite_cycle_select_variant(RestoreSpriteCycle *self,uint32_t variant)
{
    if(self->descriptor)self->variant=CY_U(self->descriptor+12u)+(variant&0xffffu)*8u;
}
RESTORE_CYCLE_ENTRY1(FUN_0055d2e0,restore_sprite_cycle_select_variant)
