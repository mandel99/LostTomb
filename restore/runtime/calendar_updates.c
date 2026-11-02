#include "calendar_updates.h"
#include "menu_event_actions.h"
#include "event_publisher.h"
#include "actor_predicate.h"
#include "flood_simulation.h"
#include <stdlib.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern int32_t FUN_004cde40(int32_t,int32_t);
extern int32_t FUN_004cde60(int32_t,int32_t);
extern void FUN_004e96c0(int32_t);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_0046e180(int32_t);
extern uint32_t FUN_004d4e20(int32_t);
extern void FUN_004e81d0(void);
extern void FUN_004474e0(void);
extern void FUN_0045e420(void);
extern void FUN_004464f0(void);
extern void FUN_004ea740(void);
extern void FUN_004b7960(void);
extern void FUN_004e1850(void);
extern void FUN_0047b570(void);
extern void FUN_00517370(void);
extern void FUN_004e1640(void);
extern void FUN_004e7700(void);
extern void FUN_004bc7a0(void);
extern void FUN_004e8630(void);
extern void FUN_004448e0(void);
extern void FUN_00455530(void);
extern void FUN_004c5300(void);
extern void FUN_004e9700(void);
extern void FUN_004ea000(void);
extern void FUN_004e8840(void);
extern void FUN_004e8db0(void);

static uint32_t city_offset(void){return U(0xeaac28u)*0x49d8u;}

/* Whole452160/41 instructions. Its sole caller discards EAX, so the
   logical API publishes state/messages instead of an unresolved return. */
void FUN_00452160(void)
{
    uint32_t offset=city_offset(),months,years,index;
    if((int32_t)U(0xea187cu+offset)>=0){
        U(0xea609cu+offset)=0;U(0xea60a0u+offset)=0;return;
    }
    months=U(0xea609cu+offset)+1u;U(0xea609cu+offset)=months;
    if(months==0||(int32_t)months%12!=0)return;
    years=U(0xea60a0u+offset)+1u;U(0xea60a0u+offset)=years;
    if((int32_t)years>=10)years=10;
    index=years+U(0xe39080u)*10u;
    FUN_004e96c0((int32_t)U(0x5da330u+index*4u));
    U(0xe950dcu)=1;FUN_004e0cf0(0x79,0,0,0,0);
}

/* Whole452200/97 instructions. Two signed WORD building references per
   actor are resolved with the actual264-byte building record stride. */
void FUN_00452200(void)
{
    static const uint32_t flags[4]={0x6f936cu,0x6f5694u,0x6dea2cu,0x6e91d4u};
    uint8_t found[4]={0,0,0,0},previous[4];
    uint32_t record,index;
    for(index=0;index<4u;++index)previous[index]=(uint8_t)(B(flags[index])!=0);
    for(record=0xbcd630u;record<0xc8ad70u;record+=388u){
        if(B(record-46u)==0x55u){
            uint8_t state=B(record);
            if(state==15u||state==16u||state==36u){
                uint32_t ref;
                for(ref=0;ref<2u;++ref){
                    int32_t building=(int16_t)W(record+36u+ref*2u);
                    uint16_t kind=W(0x7999e8u+(uint32_t)building*264u);
                    switch(kind){
                    case 0xe5:found[0]=1;break;
                    case 0xea:found[1]=1;break;
                    case 0xeb:found[2]=1;break;
                    case 0xec:found[3]=1;break;
                    }
                }
            }
        }
    }
    for(index=0;index<4u;++index)if(previous[index]&&!found[index])B(flags[index])=0;
}

/* Whole4BE640/79 instructions. Native sole caller45253D sets ECX=D05720;
   quality uses the genuine explicit receiver at self+32C40. */
void FUN_004be640(void)
{
    uint32_t self=0xd05720u,category,message;
    int32_t quality;
    if(U(self+0x32fb4u)!=5u||B(self+0x32fbcu)==0)return;
    U(0xe950dcu)=1;
    quality=restore_flood_quality(self+0x32c40u);
    category=(uint32_t)((int32_t)((uint32_t)quality+19u)/20);
    if(category>5u)return;
    switch(category){
    case 0:message=0x158;break;
    case 1:message=W(0x7848e2u)?0x159u:0x157u;break;
    case 2:message=W(0x7848e2u)?0x15au:0x156u;break;
    case 3:message=0x15b;break;
    case 4:message=0x15c;break;
    default:message=0x15d;break;
    }
    FUN_004e0cf0((int32_t)message,0,0,0,0);
}

/* Whole459E50/24 instructions, including the complete active-house scan. */
void FUN_00459e50(void)
{
    uint32_t o=city_offset(),v=U(0xea44b0u+o),count,record;
    v+=U(0xea44acu+o);U(0xea44c0u+o)=v;count=U(0x78d790u);
    U(0xea44acu+o)=0;U(0xea44b0u+o)=0;U(0xea44b4u+o)=0;U(0xea44b8u+o)=0;
    if((int32_t)count<1)return;
    record=0x799ae5u;
    do{
        if(B(record-5u)==1u&&B(record)!=0)U(record+0x87u)=0;
        record+=264u;
    }while(--count);
}
/* Whole45A2E0/25 instructions. Both native magic divisions truncate toward
   zero after the low32-bit signed wage product, first/10 then/12. */
void FUN_0045a2e0(void)
{
    uint32_t o=city_offset(),product=U(0xea4464u+o),cost;
    product*=U(0xea4478u+o);
    cost=(uint32_t)(((int32_t)product/10)/12);
    U(0xea187cu+o)=U(0xea187cu+o)-cost;
    U(0xea4484u+o)=U(0xea4484u+o)+cost;
    U(0xea5d4cu+o)=U(0xea5d4cu+o)+U(0xea4478u+o);
}
/* Whole45A3F0/18 instructions. Retain each fresh salary-value load. */
void FUN_0045a3f0(void)
{
    uint32_t o=city_offset(),v,w;
    if((int32_t)U(0xea187cu+o)<=-5000||U(0xea620cu+o)==0)return;
    v=U(0xea4508u+o);w=U(0xea4510u+o);U(0xea4510u+o)=w+v;
    v=U(0xea4508u+o);w=U(0xea5cc0u+o);U(0xea5cc0u+o)=w+v;
    v=U(0xea4508u+o);U(0xea187cu+o)=U(0xea187cu+o)-v;
}
static void rollover(uint32_t o,uint32_t source,uint32_t destination)
{U(destination+o)=U(source+o);U(source+o)=0;}
/* Complete native annual rollover bodies:10/10/6/6/6/11/6 instructions. */
void FUN_0045a450(void)
{uint32_t o=city_offset();rollover(o,0xea4484u,0xea448cu);rollover(o,0xea5d4cu,0xea5e18u);}
void FUN_0045a490(void)
{uint32_t o=city_offset();rollover(o,0xea44d4u,0xea44d8u);rollover(o,0xea44dcu,0xea44e0u);}
void FUN_0045a4d0(void)
{rollover(city_offset(),0xea44fcu,0xea44f8u);}
void FUN_0045a500(void)
{rollover(city_offset(),0xea44e4u,0xea44e8u);}
void FUN_0045a530(void)
{rollover(city_offset(),0xea4510u,0xea4500u);}
void FUN_0045a560(void)
{
    uint32_t o=city_offset();rollover(o,0xea44f4u,0xea44f0u);
    W(0xea5cfeu+o)=W(0xea5cfcu+o);W(0xea5cfcu+o)=0;U(0xea5ff4u+o)=0;
}
void FUN_0045a5a0(void)
{rollover(city_offset(),0xea5fc8u,0xea5fc4u);}

/* Whole452370/149 instructions. Calendar16-step month/12-month year,
   exact monthly/yearly provider order and fresh global reloads retained. */
void FUN_00452370(void)
{
    uint32_t day=U(0x78d7c4u),total=U(0x78d7c8u),o,player,v;
    ++total;++day;U(0x78d7c8u)=total;U(0x78d7c4u)=day;
    if((int32_t)day>15){
        o=city_offset();U(0x78d7c4u)=0;
        U(0xea5fccu+o)=U(0xea3facu+o);U(0xea3facu+o)=0;
        U(0xea5fd0u+o)=0;U(0xea5fd4u+o)=0;U(0xea5bfcu+o)=U(0xea5bfcu+o)+1u;
        FUN_004e81d0();FUN_004599d0();FUN_0045a2e0();FUN_0045a350();FUN_0045a3f0();
        FUN_00452160();FUN_004474e0();FUN_0045e420();FUN_004464f0();FUN_0044ba90();
        FUN_004ea740();FUN_004b7960();FUN_004e1850();FUN_0047b570();
        v=U(0xbca2d4u)-1u;o=U(0xbca2d0u)-1u;
        FUN_00475cc0(0,0,(int32_t)o,(int32_t)v,0);
        FUN_00517370();FUN_004e1640();U(0x78d7c0u)=U(0x78d7c0u)+1u;
        FUN_004e7700();FUN_004bc7a0();FUN_00452200();
        if((int32_t)U(0x78d7c0u)>11){
            U(0x78d7c0u)=0;
            /* Native51F780 is the genuine single-byte RET primitive. */
            v=U(0x78d7bcu);B(0xe3908cu)=0;U(0xbc9694u)=0;U(0x78d7bcu)=v+1u;
            FUN_004e8630();FUN_00459e50();FUN_0045a450();FUN_0045a490();FUN_0045a4d0();
            FUN_0045a500();FUN_0045a530();FUN_0045a560();FUN_0045a5a0();FUN_0045a5d0();
            FUN_004448e0();FUN_00455530();FUN_0046e180(1);FUN_004c5300();
        }else FUN_0046e180(0);
        v=U(0x78d7c0u);
        if(v==0||v==6)U(0xea5f8cu+city_offset())=2;
        FUN_004e9700();FUN_004ea000();
        if(B(0xe3908cu)==0&&B(0xe38ec2u)==1)(void)FUN_004d4e20(0x5dbb94);
    }
    v=U(0x78d7c4u);
    if(v==0||v==8){
        FUN_004e8840();
        if(U(0x78d7c4u)==8)FUN_004be640();
    }
    FUN_004e8db0();v=B(0x7027b8u);player=U(0xeaac28u);
    if(v&&U(0xea5f90u+player*0x49d8u)==0){
        U(0xea5f90u+player*0x49d8u)=1;U(0xe950dcu)=1;
    }
    if(B(0x7027beu)&&U(0xea5f94u+player*0x49d8u)==0){
        U(0xea5f94u+player*0x49d8u)=1;U(0xe950dcu)=1;
        FUN_004e0cf0(0x13e,0,0,0,0);player=U(0xeaac28u);
    }
    if(B(0x7027b9u)==0&&(int32_t)U(0xea1894u+player*0x49d8u)>=150)FUN_00417ae0(1);
}
