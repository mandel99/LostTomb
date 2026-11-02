#include "sprite_cycle_numeric.h"
void restore_sprite_cycle_select_frame(RestoreSpriteCycle *self,uint32_t variant,int32_t frame)
{
    if(!self->descriptor)return;
    restore_sprite_cycle_select_variant(self,variant);
    cycle_integer_frame(self,(int16_t)frame);
}
RESTORE_CYCLE_ENTRY2(FUN_0055d300,restore_sprite_cycle_select_frame)
