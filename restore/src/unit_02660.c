#include "sprite_cycle_numeric.h"
void restore_sprite_cycle_set_frame(RestoreSpriteCycle *self,int32_t input)
{
    int16_t frame=(int16_t)input;
    uint16_t count_value;
    if(!self->variant)return;
    count_value=CY_W(self->variant+2u);
    while(frame<0)frame=(int16_t)((uint16_t)frame+count_value);
    while((int32_t)frame>=count_value)frame=(int16_t)((int32_t)frame%count_value);
    cycle_integer_frame(self,frame);
}
RESTORE_CYCLE_ENTRY1(FUN_0055d290,restore_sprite_cycle_set_frame)
