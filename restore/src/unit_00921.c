#include "city_worker_updates.h"
#include "actor_create.h"
#include "actor_work.h"
#include "menu_resource_neighbors.h"
#include "city_updates.h"

#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern int32_t FUN_004cde60(int32_t value,int32_t divisor);

/* 4615D0: kind80 service worker, clearing both the signed stock WORD
   and the delay WORD after the patrol callback and worker-count update. */
void restore_city_update_stock_reset_worker(int32_t building)
{
    uint32_t b=(uint32_t)building*264u,a,spawn_x,spawn_y;
    uint8_t orientation,player,workers;
    int32_t x,y,kind,employed,maximum,percentage,limit,actor;

    if((int16_t)W(0x7999f0u+b)<=0)B(0x799a9bu+b)=2;
    y=(int16_t)W(0x7999e0u+b);
    x=(int16_t)W(0x7999deu+b);
    orientation=B(0x7999dbu+b);
    if(FUN_00480180(x,y,orientation)==0u)return;
    if((int16_t)W(0x7999f0u+b)<=50)FUN_00467220(building);
    kind=(int16_t)W(0x7999e8u+b);
    employed=(int16_t)W(0x799a18u+b);
    maximum=(int32_t)U(0x705eb8u+(uint32_t)kind*40u);
    percentage=FUN_004cde60(employed,maximum);
    if(percentage>=100)limit=4;
    else if(percentage>=75)limit=3;
    else if(percentage>=50)limit=2;
    else {
        if(percentage<25)return;
        limit=1;
    }
    workers=B(0x799a7eu+b);
    if((int32_t)workers>=limit)return;
    B(0x799a0au+b)=0;
    workers=B(0x799a7eu+b);
    if((int32_t)workers>=limit)return;
    spawn_y=U(0xd3b4e0u);
    spawn_x=U(0xd3b4e4u);
    player=B(0x7999d9u+b);
    actor=FUN_004b4800(player,80,(int32_t)spawn_x,(int32_t)spawn_y,0);
    a=(uint32_t)actor*388u;
    B(0xbcd630u+a)=8;
    W(0xbcd652u+a)=(uint16_t)building;
    W(0xbcd62eu+a)=0;
    FUN_004b1df0(actor);
    workers=B(0x799a7eu+b);
    workers=(uint8_t)(workers+1u);
    W(0xbcd63cu+a)=0;
    B(0x799a7eu+b)=workers;
    W(0x799a2cu+b)=0;
    W(0x799a14u+b)=0;
}
