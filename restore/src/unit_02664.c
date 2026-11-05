#include "sprite_controller.h"

void restore_sprite_cycle_set_action(RestoreSpriteCycle *self,uint32_t action)
{
    restore_sprite_cycle_select_action(self,action,0,NULL);
}
