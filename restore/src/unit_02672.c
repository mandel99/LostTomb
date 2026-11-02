#include "sprite_cycle.h"
uint32_t restore_sprite_cycle_finished(RestoreSpriteCycle *self)
{return restore_sprite_cycle_tick(self)!=0;}
RESTORE_CYCLE_ENTRY0(FUN_0055d760,restore_sprite_cycle_finished)
