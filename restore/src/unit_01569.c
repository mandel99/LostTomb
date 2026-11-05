#include "city_worker_supply.h"
#include "city_updates.h"
#include "actor_create.h"
#include "menu_resource_neighbors.h"
#include "menu_list_scroll.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))

/* Whole4BED70 has physical RET4. This logical memory-effect API uses explicit
   C arguments; unchanged RAW preserves native stack/register behavior. */
void restore_city_dispatch_kind85(int32_t building)
{
    uint32_t off=(uint32_t)building*264u,limit,period,x,y,size,actor,actor_off;
    int32_t kind,amount,percentage;
    uint8_t timer,player,count;
    if(H(0x7999f0u+off)<=0)B(0x799a9bu+off)=2u;
    y=(uint32_t)H(0x7999e0u+off);x=(uint32_t)H(0x7999deu+off);size=B(0x7999dbu+off);
    if(FUN_00480180((int32_t)x,(int32_t)y,size)==0u)return;
    if(H(0x7999f0u+off)<=50)FUN_00467220(building);
    kind=H(0x7999e8u+off);amount=H(0x799a18u+off);
    limit=U(0x705eb8u+(uint32_t)kind*40u);
    percentage=FUN_004cde60(amount,(int32_t)limit);
    if(percentage>=100)period=3u;
    else if(percentage>=75)period=7u;
    else if(percentage>=50)period=15u;
    else if(percentage>=25)period=29u;
    else if(percentage>=1)period=47u;
    else return;
    if(B(0x799a7eu+off)>=4u)return;
    timer=(uint8_t)(B(0x799a0au+off)+1u);B(0x799a0au+off)=timer;
    if((uint32_t)timer<=period)return;
    y=U(0xd3b4e0u);x=U(0xd3b4e4u);
    B(0x799a0au+off)=0u;player=B(0x7999d9u+off);
    actor=(uint32_t)FUN_004b4800(player,85,(int32_t)x,(int32_t)y,0);
    W(0x799a02u+off)=(uint16_t)actor;actor_off=actor*388u;
    count=B(0x799a7eu+off);
    B(0xbcd630u+actor_off)=8u;B(0x799a7eu+off)=(uint8_t)(count+1u);
    W(0xbcd652u+actor_off)=(uint16_t)building;
}
