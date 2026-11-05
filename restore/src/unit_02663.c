#include "sprite_controller.h"

#define SC_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SC_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define SC_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

static uint16_t sprite_count_compare(uint32_t count,RestoreSpriteCycle *self)
{
    uint16_t status;
    __asm { fild count }
    __asm { mov eax,self }
    __asm { fcomp dword ptr[eax+48h] }
    __asm { fnstsw status }
    return status;
}
static void sprite_action_rate(RestoreSpriteCycle *self,uint32_t rate)
{
    __asm { fild rate }
    __asm { mov eax,self }
    __asm { fstp dword ptr[eax+50h] }
}

void restore_sprite_cycle_select_action(RestoreSpriteCycle *self,
    uint32_t requested,uint32_t frame,uint32_t *native_action_slot)
{
    uint32_t base=(uint32_t)(uintptr_t)self,source,data,descriptor;
    uint32_t variant,mode,repeats,count,rate,directions,variants;
    uint32_t middle,first,width,height,anchor;
    uint16_t action=(uint16_t)requested,status;
    source=SC_U(base+0x38u);
    if(!source)return;
    if(action){
        data=SC_U(source+8u);
        count=SC_W(data+0x1au);
        if(action>=count)action=(uint16_t)(SC_W(data+0x1au)-1u);
    }
    SC_U(base+0x34u)=SC_U(base+0x34u)&0xfffffeffu;
    source=SC_U(base+0x38u);
    SC_W(base+0x3cu)=action;
    data=source?SC_U(source+8u):0;
    descriptor=SC_U(data+0x1cu)+(uint32_t)action*16u;
    SC_U(base+0x40u)=descriptor;
    /* Both native paths reach 55D2E0's AND index,0xffff. The reconstructed
       logical direction API therefore returns the observed WORD index. */
    variant=restore_sprite_cycle_direction(self);
    if((uint16_t)frame==1000u)restore_sprite_cycle_select_variant(self,variant);
    else restore_sprite_cycle_select_frame(self,variant,(int16_t)frame);
    descriptor=SC_U(base+0x40u);
    mode=SC_B(descriptor);
    repeats=SC_B(descriptor+3u);
    SC_W(base+0x78u)=(uint16_t)(repeats<<((mode&7u)==4u));
    /* 55D750 is XOR AX,AX;RET. Its retained high16 is dead because the
       complete set-frame method consumes only a signed low16 frame. */
    if((uint16_t)frame==0)restore_sprite_cycle_set_frame(self,0);
    variant=SC_U(base+0x44u);
    count=SC_W(variant+2u);
    if(native_action_slot)*(volatile uint32_t *)native_action_slot=count;
    status=sprite_count_compare(count,self);
    /* Native TEST AH,41h also selects the correction for unordered input. */
    if(status&0x4100u)restore_sprite_cycle_set_frame(self,(int32_t)(count-1u));
    descriptor=SC_U(base+0x40u);
    rate=SC_B(descriptor+2u);
    if(native_action_slot)*(volatile uint32_t *)native_action_slot=rate;
    sprite_action_rate(self,rate);
    directions=SC_B(descriptor+11u);
    if(directions>1u){
        variants=SC_U(descriptor+12u);
        directions=SC_B(descriptor+11u);
        middle=SC_U(variants+(directions>>1)*8u+4u);
        first=SC_U(variants+4u);
        width=SC_W(first+2u);
        SC_U(base+0x18u)=width;
        height=SC_W(middle+2u);
        SC_U(base+0x1cu)=height;
        anchor=SC_W(middle+4u);
        width=SC_U(base+0x18u);
        SC_U(base+0x20u)=anchor-(uint32_t)((int32_t)width/2);
    }
}
