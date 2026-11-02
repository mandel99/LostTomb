#include "placement_preview.h"
extern void FUN_004100f0(const void *,uint32_t),FUN_004101c0(const void *,uint32_t),FUN_00410700(const void *,uint32_t),FUN_004109d0(const void *,uint32_t),FUN_00410a70(const void *,uint32_t);
int32_t restore_placement_mask_sprite(RestoreSpriteLibraries *self,int32_t image,int32_t x,int32_t y,uint32_t mask)
{
    uint32_t row;
    if(image<=0||image>=25500)return 0;
    if(image>=(int32_t)self->next_image)image=0;
    row=(uint32_t)(uintptr_t)self+0x180e48u+(uint32_t)image*64u;
    PV_U(0xd3b5b8)=PV_U(row);if(PV_I(0xd3b5b8)<=0)return 0;
    PV_W(0xd3b5be)=PV_W(row+20);PV_W(0xd3b5bc)=PV_W(row+22);PV_B(0xd3b5b4)=PV_B(row+50);
    PV_I(0xd3b5e0)=x;PV_I(0xd3b5dc)=y;PV_I(0xd3b544)=PV_S(0xd3b5be);
    FUN_004cdad0();FUN_004cdb80();if(PV_B(0xd3b530)==5)return 0;
    if(!PV_B(row+51)) {
        if(PV_B(0xd3b531)==1||PV_B(0xd3b531)==2)FUN_004101c0((void *)(uintptr_t)PV_U(0xd3b5b8),mask);
        else FUN_004100f0((void *)(uintptr_t)PV_U(0xd3b5b8),mask);
    } else {
        if(PV_B(0xd3b531)==1)FUN_004109d0((void *)(uintptr_t)PV_U(0xd3b5b8),mask);
        else if(PV_B(0xd3b531)==2)FUN_00410a70((void *)(uintptr_t)PV_U(0xd3b5b8),mask);
        else FUN_00410700((void *)(uintptr_t)PV_U(0xd3b5b8),mask);
    }
    return PV_I(0xd3b544);
}
/* All34 retained native callers immediately MOV ECX,F55BB0 before CALL. */
int32_t FUN_0053f4f0(int32_t image,int32_t x,int32_t y,uint32_t mask)
{return restore_placement_mask_sprite(RESTORE_SPRITE_LIBRARIES,image,x,y,mask);}
