/* Complete native4BD950: one floodplain cell entering/leaving the flood.
   Explicit ECX receiver; original RET4 is normalized to a C parameter. */
#include "flood_simulation.h"
#include "actor_walking.h"
#include "building_refresh_helpers.h"
#include "campaign_score_numeric.h"
#include "menu_building_kind.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_0047b590(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047bae0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_0047bd50(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00517690(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00516f90(int32_t,int32_t,int32_t,int32_t,uint8_t);
extern void FUN_005173a0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047db00(int32_t);
extern void FUN_004bd250(int32_t);

static uint8_t fertility(uint32_t receiver)
{
    int32_t quality=(int32_t)U(U(receiver)+8u);
    uint8_t raw80[10];
    __asm {
        fild quality
        fmul qword ptr ds:[0x56f528]
        fmul qword ptr ds:[0x56f660]
        lea eax,raw80
        fstp tbyte ptr[eax]
    }
    return (uint8_t)restore_campaign_score_trunc80(raw80);
}

static void refresh(uint32_t receiver,int flooding,int water)
{
    int32_t x=(int32_t)U(receiver+12u),y=(int32_t)U(receiver+16u);
    int radius=water&&!flooding?3:2,middle=water?2:1;
    if(water){
        FUN_00517690(x-1,y-1,x+1,y+1);
        FUN_00516f90(x-1,y-1,x+1,y+1,0);
        FUN_005173a0(x-1,y-1,x+1,y+1);
    }
    if(!water)FUN_004777c0(x,y,x+1,y+1);
    else {int inner=flooding?1:2;FUN_004777c0(x-inner,y-inner,x+inner,y+inner);}
    FUN_00475cc0(x-middle,y-middle,x+middle,y+middle,0);
    FUN_0047b590(x-radius,y-radius,x+radius,y+radius);
    FUN_0047bae0(x-radius,y-radius,x+radius,y+radius,0);
}

/* These two members forward the actual world receiver into the shared,
   complete building-query/draw graph. Do not use receiver-less legacy APIs. */
static uint8_t building_member(uint32_t entry,int32_t building)
{
    uint32_t stack[256],sp=(uint32_t)(uintptr_t)(stack+224);
    struct restore_building_refresh_frame f={{0},2,restore_building_refresh_dispatch};
    stack[224]=0;stack[225]=(uint32_t)building;stack[226]=0;
    f.r[1]=0xbca350u;f.r[4]=sp;
    restore_building_refresh_dispatch(0,entry,&f);
    if(f.r[4]!=sp+(entry==0x488e60u?12u:8u))abort();
    return (uint8_t)f.r[0];
}

static void move_from_receding_water(int32_t id,uint32_t tile)
{
    static const int32_t dx[8]={0,1,1,1,0,-1,-1,-1};
    static const int32_t dy[8]={-1,-1,0,1,1,1,0,-1};
    uint32_t a=(uint32_t)id*388u,j,direction;
    int32_t run=0,start=-1,best=0,best_start=-1;
    U(0xbc9768u)=tile;FUN_0047db00(4);
    for(j=0;j<16u;++j){
        uint32_t open=U(0xbc97c0u+(j&7u)*4u);
        if(open){if(start<0)start=(int32_t)j;++run;}
        /* Native clears a run only when it becomes the new maximum. */
        if((!open||j==15u)&&run>best){best=run;best_start=start;start=-1;run=0;}
    }
    direction=((uint32_t)(best/2)+(uint32_t)best_start)&7u;
    FUN_004b5420(id);
    W(0xbcd642u+a)=(uint16_t)(W(0xbcd642u+a)+(uint32_t)dx[direction]*32u);
    W(0xbcd60cu+a)=(uint16_t)(W(0xbcd60cu+a)+(uint32_t)dx[direction]*2u);
    W(0xbcd644u+a)=(uint16_t)(W(0xbcd644u+a)+(uint32_t)dy[direction]*32u);
    W(0xbcd60eu+a)=(uint16_t)(W(0xbcd60eu+a)+(uint32_t)dy[direction]*2u);
    U(0xbcd618u+a)+=(uint32_t)(dx[direction]+dy[direction]*228)*2u;
    FUN_004b52f0(id);
}

void FUN_004bd950(uintptr_t self,uint8_t flooding)
{
    uint32_t receiver=(uint32_t)self,tile=U(receiver+8u),flags=U(0xaf598cu+tile*4u);
    int32_t building=H(0xb4e6fcu+tile*2u),id;
    if(flooding){
        uint32_t row=(uint32_t)building*264u;
        B(0xadc36cu+tile)=0;U(0xaf598cu+tile*4u)=(flags&0xffffefffu)|4u;
        B(0xa5d4ccu+tile)=B(receiver+24u)?fertility(receiver):0;
        if(flags&0x40u)U(0xaf598cu+tile*4u)=(flags&0xffffefbfu)|0x8000004u;
        refresh(receiver,1,1);
        if((flags&8u)&&building){
            int32_t kind=H(0x7999e8u+row);
            if(kind!=99&&kind!=88&&kind!=89&&kind!=93&&kind!=81&&kind!=90&&kind!=138&&B(0x7999d8u+row)!=6){
                if(!(uint8_t)FUN_00488030((uint32_t)building)){
                    B(0x7999d8u+row)=6;B(0x799a97u+row)=1;
                }else if(U(0xea6208u+U(0xeaac28u)*0x49d8u)){
                    U(0xea6208u+U(0xeaac28u)*0x49d8u)=2;B(0x7999d8u+row)=6;
                }else{
                    uint32_t at=U(0xbca2d8u)+(uint32_t)H(0x7999deu+row)+(uint32_t)H(0x7999e0u+row)*228u,y,x;
                    int intact=1;
                    for(y=0;y<3u&&intact;++y,at+=228u)
                        for(x=0;x<3u;++x){
                            uint32_t value=U(0xaf598cu+(at+x)*4u);
                            if(value&8u){B(0xb285ccu+at+x)=0;U(0xaf598cu+(at+x)*4u)=value&~8u;}
                            else intact=0;
                        }
                    W(0x799a2cu+row)=0;W(0x799a18u+row)=0;
                    B(0x799a7eu+row)=0;B(0x799a7fu+row)=0;
                    {int32_t px=(int32_t)U(receiver+12u),py=(int32_t)U(receiver+16u);
                     FUN_004777c0(px-2,py-2,px+2,py+2);FUN_00475cc0(px-2,py-2,px+2,py+2,0);}
                }
            }
        }
        id=H(0xb350dcu+tile*2u);
        while(id){
            uint32_t a=(uint32_t)id*388u;
            if(H(0xbcd600u+a)==id)break;
            if(B(0xbcd602u+a)!=14&&B(0xbcd602u+a)!=28)FUN_004bd250(id);
            id=H(0xbcd600u+a);
        }
    }else{
        if(flags&4u){
            U(0xaf598cu+tile*4u)=flags&0xfbfffffbu;
            if(flags&0x8000000u)U(0xaf598cu+tile*4u)=(flags&0xf3fffffbu)|0x40u;
            {int32_t x=(int32_t)U(receiver+12u),y=(int32_t)U(receiver+16u);
             FUN_0047bd50(x-3,y-3,x+3,y+3);}
            refresh(receiver,0,1);
            id=H(0xb350dcu+tile*2u);
            while(id){
                uint32_t a=(uint32_t)id*388u;uint8_t kind;
                if(H(0xbcd600u+a)==id)break;
                kind=B(0xbcd602u+a);
                if(kind==20||kind==25||kind==77||kind==78||kind==100||kind==101)
                    move_from_receding_water(id,tile);
                /* Native reloads the link after relocation, even after unlink. */
                id=H(0xbcd600u+a);
            }
        }else refresh(receiver,0,0);
        if((uint8_t)FUN_00488030((uint32_t)building)&&!building_member(0x4894d0u,building)){
            uint32_t row=(uint32_t)building*264u;
            uint32_t at=U(0xbca2d8u)+(uint32_t)H(0x7999deu+row)+(uint32_t)H(0x7999e0u+row)*228u,y,x;
            for(y=0;y<3u;++y,at+=228u){
                W(0xb285ccu+at)=0x202;B(0xb285ccu+at+2u)=2;
                for(x=0;x<3u;++x)U(0xaf598cu+(at+x)*4u)|=8u;
            }
            (void)building_member(0x488e60u,building);
        }
    }
}
