#include "sprite_cycle_numeric.h"
uint32_t restore_sprite_cycle_wrap_forward(RestoreSpriteCycle *self)
{
    float count_value=cycle_length(self);
    uint32_t bits,finished=0,less;
    __asm { mov eax,count_value }
    __asm { mov bits,eax }
    while(!cycle_less(self,count_value)&&!finished){
        less=cycle_sum(self,bits,1,count_value);
        if(self->remaining){
            self->remaining=(uint16_t)(self->remaining-1u);
            if(!self->remaining){
                if(!less)cycle_last_frame(self,count_value);
                self->remaining=cycle_repeat(self);finished=1;
            }
        }
    }
    return finished;
}
RESTORE_CYCLE_ENTRY0(FUN_0055d590,restore_sprite_cycle_wrap_forward)
