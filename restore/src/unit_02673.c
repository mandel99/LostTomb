#include "sprite_cycle.h"
uint32_t restore_sprite_cycle_stop_when_finished(RestoreSpriteCycle *self)
{
    if(!restore_sprite_cycle_tick(self))return 0;
    self->flags|=8u;
    return 1;
}
RESTORE_CYCLE_ENTRY0(FUN_0055d780,restore_sprite_cycle_stop_when_finished)
