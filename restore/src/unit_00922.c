#include "city_worker_updates.h"
#include "actor_create.h"
#include "actor_work.h"
#include "actor_order_action.h"
#include "actor_laborer.h"
#include "menu_resource_neighbors.h"
#include "city_updates.h"

#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern int32_t FUN_004cde60(int32_t value,int32_t divisor);

/* 461720: kind108 worker tries stock at+CC before stock at+3C.
   A successful approach consumes100 from its WORD stock and clears the
   callback-selected actor's counter; failed approaches keep the new actor. */
void restore_city_update_dual_stock_worker(int32_t building)
{
    uint32_t b=(uint32_t)building*264u,a,current,spawn_x,spawn_y;
    uint8_t orientation,player,workers;
    int32_t x,y,kind,employed,maximum,percentage,limit,actor,remaining;

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
    actor=FUN_004b4800(player,108,(int32_t)spawn_x,(int32_t)spawn_y,0);
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
    if((int16_t)W(0x799aa4u+b)>=100) {
        y=(int16_t)W(0xbcd60eu+a);
        x=(int16_t)W(0xbcd60cu+a);
        if(FUN_004bc2e0(x,y,actor)!=0) {
            B(0xbcd630u+a)=9;
            FUN_004b3250(actor);
            current=U(0xd056f8u);
            W(0x799aa4u+b)=(uint16_t)(W(0x799aa4u+b)-100u);
            remaining=(int16_t)W(0x799aa4u+b);
            W(0xbcd63cu+current*388u)=0;
            if(remaining<0)W(0x799aa4u+b)=0;
            return;
        }
    }
    if((int16_t)W(0x799a14u+b)>=100) {
        y=(int16_t)W(0xbcd60eu+a);
        x=(int16_t)W(0xbcd60cu+a);
        if(FUN_004bc3b0(x,y,actor)!=0) {
            B(0xbcd630u+a)=9;
            FUN_004b3250(actor);
            current=U(0xd056f8u);
            W(0x799a14u+b)=(uint16_t)(W(0x799a14u+b)-100u);
            remaining=(int16_t)W(0x799a14u+b);
            W(0xbcd63cu+current*388u)=0;
            if(remaining<0)W(0x799a14u+b)=0;
            return;
        }
    }
    B(0xbcd65du+a)=1;
    B(0xbcd630u+a)=4;
}
