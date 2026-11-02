#include "sprite_cycle_numeric.h"
#include "map_x87_conversion.h"
uint32_t restore_sprite_cycle_hold_last_frame(RestoreSpriteCycle *self)
{
    uint8_t frame80[10];
    int32_t frame;
    __asm { mov eax,self }
    __asm { fld dword ptr[eax+48h] }
    __asm { lea eax,frame80 }
    __asm { fstp tbyte ptr[eax] }
    frame=(int16_t)(uint32_t)restore_map_trunc80(frame80);
    if(frame==(int32_t)CY_W(self->variant+2u)-1){self->flags|=0x100u;return 1;}
    if(!restore_sprite_cycle_tick(self))return 0;
    restore_sprite_cycle_set_frame(self,(uint16_t)(CY_W(self->variant+2u)-1u));
    return 1;
}
RESTORE_CYCLE_ENTRY0(FUN_0055d7b0,restore_sprite_cycle_hold_last_frame)
