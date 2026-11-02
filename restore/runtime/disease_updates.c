#include "disease_updates.h"
#include "population_updates.h"
#include "event_publisher.h"
#include "actor_create.h"
#include "actor_work.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole42FB30. The building is cached, the city is passed explicitly, and
   the mode is captured after population adjustment but before house stores. */
void FUN_0042fb30(int32_t building)
{
    uint32_t off=(uint32_t)building*264u,mode;
    FUN_004e90f0(I(U(0xeaac28u)),-H(0x7999f4u+off));mode=U(0xe38ebcu);
    W(0x7999f4u+off)=0;B(0x799a5fu+off)=1;U(0x799ac8u+off)=2;
    U(0x799ad4u+off)=0;U(0x799ac0u+off)=0;
    if(!mode&&(int32_t)(int8_t)B(0xe38ea8u)==I(U(0x579558u))&&!B(0x7027beu))FUN_00417ae0(6);
}

/* Whole42FE90, AL only. A failed actor allocation still reports success. */
uint8_t FUN_0042fe90(int32_t building)
{
    uint32_t owner,off,record;int32_t actor;
    if(!building)return 0;
    FUN_0042fb30(building);owner=U(0xeaac28u);off=owner*0x49d8u;
    ++U(0xea61f0u+off);record=(uint32_t)building*264u;
    actor=FUN_004b4800((uint8_t)owner,98,H(0x7999deu+record),H(0x7999e0u+record),0);
    if(actor){
        off=(uint32_t)actor*388u;W(0xbcd62eu+off)=500;W(0xbcd652u+off)=(uint16_t)building;
        B(0xbcd630u+off)=8;FUN_004b1df0(actor);
    }
    return 1;
}

/* Whole42FF20. The scan globals are live after each infection callback.
   Unlike ordinary services, any nonzero signed population is eligible. */
void FUN_0042ff20(int32_t x,int32_t y)
{
    uint32_t left=(uint32_t)x-2u,right=(uint32_t)x+2u,top=(uint32_t)y-2u,bottom=(uint32_t)y+2u;
    uint32_t width,height,tile,gap,off,next_x,next_y;int32_t building,kind;
    if(I(left)<0)left=0;if(I(top)<0)top=0;
    width=U(0xbca2d0u);if(I(right)>=I(width))right=width-1u;
    height=U(0xbca2d4u);if(I(bottom)>=I(height))bottom=height-1u;
    U(0xd3b4e0u)=top;tile=U(0xbca2d8u)+top*228u+left;gap=227u+left-right;U(0xbc9768u)=tile;
    if(I(top)>I(bottom))return;
    do{
        U(0xd3b4e4u)=left;
        if(I(left)<=I(right))do{
            building=H(0xb4e6fcu+tile*2u);off=(uint32_t)building*264u;
            if(building&&B(0x7999d8u+off)){
                kind=H(0x7999e8u+off);
                if(kind>=10&&kind<=29&&H(0x7999f4u+off)&&!B(0x799a5fu+off)){
                    FUN_0042fb30(building);tile=U(0xbc9768u);
                }
            }
            next_x=U(0xd3b4e4u)+1u;++tile;U(0xd3b4e4u)=next_x;U(0xbc9768u)=tile;
        }while(I(next_x)<=I(right));
        next_y=U(0xd3b4e0u)+1u;tile+=gap;U(0xd3b4e0u)=next_y;U(0xbc9768u)=tile;
    }while(I(next_y)<=I(bottom));
}

/* Whole42FBB0. Commit the old sample/history before loading both generators.
   The full native return retains the quotient's high bytes around SETL AL. */
uint32_t FUN_0042fbb0(void)
{
    uint32_t cursor=U(0xd3b1c4u),sample=U(0xd3b374u),low,high,a,b,i,value,owner,threshold;
    U(0xd3b1c8u+cursor*4u)=sample;++cursor;U(0xd3b1c4u)=cursor;
    if(I(cursor)>=100)U(0xd3b1c4u)=0;
    high=U(0xd3b384u);low=U(0xd3b380u);
    for(i=0;i<31u;++i){
        a=((high>>4)^high)&1u;b=((low>>4)^low)&1u;
        high>>=1;low>>=1;if(a)high|=0x40000000u;if(b)low|=0x40000000u;
    }
    U(0xd3b384u)=high;U(0xd3b378u)=high&7u;U(0xd3b380u)=low;
    U(0xd3b364u)=low&0x7fffu;U(0xd3b36cu)=low&7u;owner=U(0xeaac28u);
    U(0xd3b374u)=high&0x7fu;U(0xd3b368u)=low&0x7fu;
    threshold=40u-U(0xea1888u+owner*0x49d8u);value=high&0x7fffu;U(0xd3b370u)=value;
    return (value/100u&0xffffff00u)|(uint32_t)((int32_t)(value%100u)<I(threshold));
}

void FUN_00430040(int32_t building)
{
    FUN_0042fb30(building);U(0x70e128u)=(uint32_t)building;U(0x70e134u)=1;
}

/* Whole430060. This radius scan uses only local coordinates, not the live
   service globals. Return full native EAX; callers consume its low byte. */
uint32_t FUN_00430060(int32_t building)
{
    uint32_t off=(uint32_t)building*264u,owner,month,radius,left,right,top,bottom,width,height,tile,gap;
    uint32_t row,column,scan,other,dx,dy,forward,backward,result;
    int32_t target;
    FUN_0042fb30(building);owner=U(0xeaac28u);U(0x70e130u)=1;
    radius=(uint32_t)(I(100u-U(0xea1888u+owner*0x49d8u))/20);U(0x70e124u)=(uint32_t)building;
    month=U(0x78d7c0u);if(month==9u||month==10u||month==11u)++radius;
    if(I(radius)<1)return month&0xffffff00u;
    if(I(radius)>4)radius=4;
    left=(uint32_t)H(0x7999deu+off)-radius;right=(uint32_t)H(0x7999deu+off)+radius;
    top=(uint32_t)H(0x7999e0u+off)-radius;bottom=(uint32_t)H(0x7999e0u+off)+radius;
    if(I(left)<0)left=0;if(I(top)<0)top=0;
    width=U(0xbca2d0u);if(I(right)>=I(width))right=width-1u;
    height=U(0xbca2d4u);if(I(bottom)>=I(height))bottom=height-1u;
    tile=U(0xbca2d8u)+top*228u+left;gap=227u+left-right;result=top;
    if(I(top)<=I(bottom)){
        row=top;
        do{
            column=left;scan=tile;
            if(I(left)<=I(right)){
                tile+=right-left+1u;
                do{
                    target=H(0xb4e6fcu+scan*2u);other=(uint32_t)target*264u;
                    if(target&&B(0x7999d8u+other)&&B(0x7999ddu+other)&&H(0x7999f4u+other)){
                        forward=column-(uint32_t)H(0x7999deu+off);backward=(uint32_t)H(0x7999deu+off)-column;
                        dx=I(forward)>I(backward)?forward:backward;
                        forward=row-(uint32_t)H(0x7999e0u+off);backward=(uint32_t)H(0x7999e0u+off)-row;
                        dy=I(forward)>I(backward)?forward:backward;
                        U(0x799ac4u+other)=I(dx)>I(dy)?dx:dy;
                    }
                    ++scan;++column;
                }while(I(column)<=I(right));
            }
            tile+=gap;++row;
        }while(I(row)<=I(bottom));
        result=bottom;
    }
    return (result&0xffffff00u)|1u;
}

/* Whole430250: scan limit is live after each callback, including the last
   record. A negative nonzero timer decrements with DWORD wrap as well. */
void FUN_00430250(void)
{
    uint32_t id=1,p=0x799bccu,value;
    if(I(U(0x78d790u))<1)return;
    do{
        if(B(p-236u)&&B(p-231u)&&H(p-208u)){
            value=U(p);if(value){U(p)=--value;if(!value)FUN_0042fb30(I(id));}
        }
        ++id;p+=264u;
    }while(I(id)<=I(U(0x78d790u)));
}

/* Whole4302B0. Clear each pending flag after its publisher returns, and only
   then read the next flag, allowing the publisher to enqueue later notices. */
void FUN_004302b0(void)
{
    static const uint32_t flags[3]={0x70e138u,0x70e134u,0x70e130u};
    static const uint32_t sources[3]={0x70e12cu,0x70e128u,0x70e124u};
    static const int32_t messages[3]={0x147,0x146,0x148};uint32_t i,off;
    for(i=0;i<3u;++i)if(U(flags[i])){
        off=U(sources[i])*264u;U(0xe950dcu)=1;
        FUN_004e0cf0(messages[i],(uint32_t)H(0x7999e8u+off),U(0x7999e4u+off),0,0);U(flags[i])=0;
    }
}
