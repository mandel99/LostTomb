#include "sprite_controller.h"

#define SC_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SC_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

uint32_t restore_sprite_cycle_dispatch(RestoreSpriteCycle *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,descriptor,mode,result;
    if(!SC_U(base+0x38u))return 0;
    descriptor=SC_U(base+0x40u);
    mode=SC_B(descriptor)&7u;
    switch(mode){
    case 0:
    case 4:
        result=restore_sprite_cycle_finished(self);
        break;
    case 1:
        /* This native branch returns directly: do not normalize or clear
           the child's completion flag with the shared dispatcher store. */
        return restore_sprite_cycle_hold_last_frame(self);
    case 2:
        result=restore_sprite_cycle_stop_when_finished(self);
        break;
    case 3:
        result=restore_sprite_cycle_next_action(self);
        break;
    default:
        result=0;
        break;
    }
    /* Reload after the selected child, including its callbacks/stores. */
    if(result)SC_U(base+0x34u)=SC_U(base+0x34u)|0x100u;
    else SC_U(base+0x34u)=SC_U(base+0x34u)&0xfffffeffu;
    return result;
}
