#include "actor_widget_members.h"
#include "event_packet.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
extern void FUN_00561a2b(uint32_t allocation);
extern int32_t FUN_00447430(int32_t city,uint8_t *reversed);
extern uint32_t restore_object_lookup_u8_00449370(uint32_t self);
extern uint32_t restore_object_lookup_u8_00449380(uint32_t self);
extern uint32_t restore_object_scale_clamp_004477d0(uint32_t self,
    int32_t *native_quotient,uint32_t *native_sign);

/* 4454A0,12 instructions. Null allocation leaves the capacity untouched. */
void restore_route_release_samples_004454a0(uint32_t self)
{
    uint32_t allocation=U(self+4u);
    if(allocation){FUN_00561a2b(allocation);U(self+4u)=0;U(self+0x138u)=0;}
}

/* 449390 reuses the two complete byte lookup algorithms. Both zero-extend
   their results; the native NEG AL/SBB EAX,EAX/INC therefore returns full
   EAX zero or one, including the first-lookup-zero arm. */
uint32_t restore_event_active_trade_00449390(uint32_t self)
{
    if(!restore_object_lookup_u8_00449370(self))return 0;
    return restore_object_lookup_u8_00449380(self)==0;
}

/* 44C110,47 instructions. Its route/type values and 4477D0's clamped
   quotient are full results. The city's signed-WORD identifier, route
   enabled BYTE, and route count BYTE retain their native load widths. */
int32_t restore_event_route_months_0044c110(uint32_t self)
{
    int32_t index,slot;uint32_t route;uint8_t reversed;
    if(B(self+2u)==2u){
        index=H(self+0x58u);
        if(index<0||index>=50)return 0;
        route=0x776fb0u+(uint32_t)index*324u;
        if(B(route+0x141u)<=1u)B(route+0x142u)=0;
        if(!B(route+0x142u))return 0;
    }else{
        if(!restore_event_active_trade_00449390(self))return 0;
        index=H(self+0x1cu);
        slot=restore_resource_lookup_slot((uint32_t)index);
        /* Native performs this search for its route-flag side effects;
           its returned route index and reversed BYTE are not consumed. */
        (void)FUN_00447430(slot,&reversed);
        route=0x776fb0u+(uint32_t)index*324u;
    }
    return (int32_t)restore_object_scale_clamp_004477d0(route,0,0);
}

/* 449750,106 instructions. The logical predicate is AL. Full EAX is also
   retained without a fictitious incoming register: early failures inherit
   the real fourth pointer's upper24 bits, later mode failures retain the
   signed mode's upper24 bits, and success retains route*81's upper24 bits.
   Output writes intentionally retain native ordering and alias behavior. */
uint32_t restore_event_trade_progress_00449750(uint32_t self,int32_t *route,
    int32_t *percentage,uint8_t *special,uint32_t *unused)
{
    uint32_t result=(uint32_t)(uintptr_t)unused;int32_t months,mode,amount;
    uint8_t reversed=(uint8_t)(uintptr_t)route;
    *route=-1;*percentage=0;*special=0;*unused=0;
    if(B(self+2u)!=1u||!B(self+0x32u)||W(self+4u)!=37u)
        return result&0xffffff00u;
    (void)FUN_00447430(H(self+0x1cu),&reversed);
    *route=H(self+0x1cu);
    months=restore_event_route_months_0044c110(self);
    if(months<1)months=1;
    mode=H(self+0x30u);
    if(mode!=5&&(mode<6||mode>7))return (uint32_t)mode&0xffffff00u;
    amount=H(self+0x2eu)*100/months;
    *percentage=mode==5?100-amount:amount;
    if(*percentage<0)*percentage=0;
    else if(*percentage>100)*percentage=100;
    if(reversed)*percentage=100-*percentage;
    result=(uint32_t)*route*81u;
    *special=(uint8_t)(B(0x7770f0u+result*4u)==1u);
    *unused=0;
    return (result&0xffffff00u)|1u;
}

/* 44C9A0,25 instructions. Signed remaining months are adjusted by a BYTE
   warning lead time and the route's full clamped month estimate. */
int32_t restore_event_warning_months_0044c9a0(uint32_t self)
{
    uint32_t remaining=(uint32_t)restore_event_remaining_months(self);
    int32_t route_months=restore_event_route_months_0044c110(self),result;
    if(route_months<1)route_months=12;
    result=(int32_t)(remaining-B(self+0x61u)-(uint32_t)route_months+6u);
    return result<1?1:result;
}
