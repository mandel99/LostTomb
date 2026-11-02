#include "god_frog_effect.h"
#include "actor_create.h"
#include "building_nearest_search.h"
#include "building_quality_update.h"
#include "legacy_format.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Complete PE bodies 4E7300/4E7410/4E75A0:82/107/124 instructions.
   Random generators remain in registers for all31 steps; publish once, in
   native store order. The old BYTE bindings changed the algorithm. */
static uint32_t advance_random(uint32_t divisor)
{
    uint32_t left=U(0xd3b384u),right=U(0xd3b380u),i,remainder;
    for(i=0;i<31u;++i){
        left=(left>>1)|((((left>>4)^left)&1u)<<30);
        right=(right>>1)|((((right>>4)^right)&1u)<<30);
    }
    U(0xd3b384u)=left;
    U(0xd3b378u)=left&7u;
    U(0xd3b380u)=right;
    U(0xd3b364u)=right&0x7fffu;
    U(0xd3b370u)=left&0x7fffu;
    U(0xd3b368u)=right&0x7fu;
    U(0xd3b36cu)=right&7u;
    remainder=(left&0x7fffu)%divisor;
    U(0xd3b374u)=left&0x7fu;
    return remainder;
}
static uint32_t squared_distance(uint32_t record,int32_t x,int32_t y)
{
    uint32_t dy=(uint32_t)H(record+8u)-(uint32_t)y;
    uint32_t dx=(uint32_t)H(record+6u)-(uint32_t)x;
    uint8_t raw80[10];
    return (uint32_t)restore_building_sqrt_trunc64((int32_t)(dy*dy+dx*dx),raw80);
}
uint32_t restore_god_effect_004e75a0(int32_t building,int32_t *x,int32_t *y)
{
    uint32_t maximum=U(0xbca2d4u),width=U(0xbca2d0u);
    uint32_t record=0x7999d8u+(uint32_t)building*264u,first,second;
    int32_t first_x,first_y,second_x,second_y,origin_x,origin_y;
    if((int32_t)maximum<=(int32_t)width)maximum=width;
    origin_y=H(record+8u);origin_x=H(record+6u);
    if(restore_building_nearest_search(origin_x,origin_y,0,0,0x40000u,
            &first_x,&first_y,(int32_t)maximum,0)!=0)
        first=squared_distance(record,first_x,first_y);
    else first=maximum;
    origin_y=H(record+8u);origin_x=H(record+6u);
    if(restore_building_nearest_search(origin_x,origin_y,0,0,4u,
            &second_x,&second_y,(int32_t)maximum,0)!=0)
        second=squared_distance(record,second_x,second_y);
    else second=maximum;
    /* Equal distances deliberately produce no output writes. */
    if((int32_t)second<(int32_t)first&&(int32_t)second<(int32_t)maximum){
        *x=second_x;*y=second_y;return 1;
    }
    if((int32_t)first<(int32_t)second&&(int32_t)first<(int32_t)maximum){
        *x=first_x;*y=first_y;return 1;
    }
    return 0;
}
void restore_god_effect_004e7410(int32_t building,int32_t x,int32_t y)
{
    uint32_t actor,record,previous,index,remainder;char text[500];
    actor=(uint32_t)FUN_004b4800((uint8_t)U(0xeaac28u),106,x,y,0);
    previous=U(0xd3b374u);record=actor*388u;
    /* No native actor==0 guard: preserve its writes to record0 as well. */
    B(0xbcd630u+record)=8;
    W(0xbcd654u+record)=(uint16_t)building;
    index=U(0xd3b1c4u);
    U(0xd3b1c8u+index*4u)=previous;
    ++index;
    W(0x799a06u+(uint32_t)building*264u)=(uint16_t)actor;
    U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
    remainder=advance_random(150);
    W(0xbcd62eu+record)=(uint16_t)remainder;
    B(0xbcd65cu+record)=0;
    B(0xbcd66au+record)=6;
    /* Native 561A36 is sprintf; 56F0F8 imports OutputDebugStringA. The PE
       format at5E4958 is retained, with the original500-byte local buffer. */
    (void)FUN_00561a36(text,(const char *)(uintptr_t)0x5e4958u,(int32_t)actor,x,y,building);
    OutputDebugStringA(text);
}
void restore_god_effect_004e7300(int32_t building)
{
    uint32_t index=U(0xd3b1c4u),previous=U(0xd3b374u);int32_t x,y;
    U(0xd3b1c8u+index*4u)=previous;
    ++index;U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
    if(advance_random(100)>=50)return;
    if(restore_god_effect_004e75a0(building,&x,&y)!=0)
        restore_god_effect_004e7410(building,x,y);
}
/* Legacy callers discard7300's incidental EAX. Bind them to the same complete
   implementation instead of retaining the old unresolved-return draft. */
void FUN_004e7300(int32_t building){restore_god_effect_004e7300(building);}
void FUN_004e7410(int32_t building,int32_t x,int32_t y)
{restore_god_effect_004e7410(building,x,y);}
uint32_t FUN_004e75a0(int32_t building,int32_t *x,int32_t *y)
{return restore_god_effect_004e75a0(building,x,y);}
