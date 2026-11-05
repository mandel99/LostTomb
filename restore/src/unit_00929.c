#include "city_spawn_helpers.h"
#include "actor_teardown_model.h"
#include "actor_create.h"
#include "actor_work.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)(int16_t)W(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Native00467220: reuse the existing algorithm with byte-addressed records. */
void restore_city_ensure_labor_seeker(int32_t building)
{
    uint32_t record,actor_record,actor,cached_x,cached_y;
    int32_t existing;
    if((uint8_t)restore_actor_building_gate(building)!=0)return;
    if(I(U(0xea1894u+U(0xeaac28u)*0x49d8u))<=0)return;
    record=(uint32_t)building*264u;
    existing=S(0x799a04u+record);
    if(existing!=0){
        actor_record=(uint32_t)existing*388u;
        if(B(0xbcd606u+actor_record)==0)W(0x799a04u+record)=0;
        if(B(0xbcd602u+actor_record)!=5u)W(0x799a04u+record)=0;
        if(S(0xbcd652u+actor_record)!=building)W(0x799a04u+record)=0;
        return;
    }
    cached_y=U(0xd3b4e0u);cached_x=U(0xd3b4e4u);
    actor=(uint32_t)FUN_004b4800(B(0x7999d9u+record),5,I(cached_x),I(cached_y),0);
    actor_record=actor*388u;
    W(0x799a04u+record)=(uint16_t)actor;
    B(0xbcd630u+actor_record)=1;
    W(0xbcd652u+actor_record)=(uint16_t)building;
    FUN_004b1df0(I(actor));
    U(0xd3b4e4u)=cached_x;U(0xd3b4e0u)=cached_y;
}
