#include "actor_walking.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole4B33D0: the geometric fallback consumes AL only. A completed route
   is released before publishing direction8; its callback cannot change the
   cached actor offset. */
void FUN_004b33d0(int32_t actor)
{
    uint32_t off=(uint32_t)actor*388u;int32_t route=H(0xbcd632u+off);
    if(route<=0){
        uint8_t direction=(uint8_t)FUN_0051cba0(H(0xbcd60cu+off),H(0xbcd60eu+off),
                                              H(0xbcd61cu+off),H(0xbcd61eu+off));
        B(0xbcd609u+off)=direction;
        if(direction!=8u)B(0xbcd609u+off)=10;
    }else if(H(0xbcd634u+off)>=H(0xbcd636u+off)){
        FUN_004b3250(actor);B(0xbcd609u+off)=8;
    }else B(0xbcd609u+off)=B(0xc8ad38u+(uint32_t)route*500u+(uint32_t)H(0xbcd634u+off));
}

/* Whole4B3470: examine the current tile before consuming each remaining
   direction. The counterpart is a DWORD; only its staffing is a signed WORD. */
uint32_t FUN_004b3470(int32_t actor,int32_t building)
{
    uint32_t off=(uint32_t)actor*388u,building_off=(uint32_t)building*264u;
    uint32_t tile=U(0xbcd618u+off),other;int32_t step=H(0xbcd634u+off),length=H(0xbcd636u+off);
    while(step<length){
        int32_t current=H(0xb4e6fcu+tile*2u);
        if(W(0x7999e8u+(uint32_t)current*264u)==136u){
            other=U(0x799a9cu+building_off);
            if((uint32_t)current==other)
                return H(0x799a18u+building_off)>0&&H(0x799a18u+other*264u)>0?1u:0u;
        }
        other=B(0xc8ad38u+(uint32_t)H(0xbcd632u+off)*500u+(uint32_t)step);
        tile+=U(0x5f80fcu+other*4u);++step;
    }
    return 2;
}

/* Whole4B3C60 and its three native jump tables. Old coordinates are captured
   before unlinking; direction and tile are read again after callbacks. */
void FUN_004b3c60(int32_t actor)
{
    static const int8_t dx[8]={0,1,1,1,0,-1,-1,-1};
    static const int8_t dy[8]={-1,-1,0,1,1,1,0,-1};
    static const int32_t delta[8]={-228,-227,1,229,228,227,-1,-229};
    static const uint8_t bridge[6]={15,27,14,15,30,14};
    uint32_t off=(uint32_t)actor*388u,tile,terrain,direction,kind,sprite;
    int32_t old_x=H(0xbcd60cu+off),old_y=H(0xbcd60eu+off);
    if((int8_t)B(0xbcd609u+off)>=8)return;
    FUN_004b5420(actor);
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+off);
    if(direction>7u)return;
    if(dy[direction])W(0xbcd60eu+off)=(uint16_t)(W(0xbcd60eu+off)+dy[direction]);
    if(dx[direction])W(0xbcd60cu+off)=(uint16_t)(W(0xbcd60cu+off)+dx[direction]);
    U(0xbcd618u+off)+=(uint32_t)delta[direction];
    FUN_004b52f0(actor);
    tile=U(0xbcd618u+off);terrain=U(0xaf598cu+tile*4u);
    if(!(terrain&0x40000040u))B(0xbcd639u+off)=0;
    else{
        B(0xbcd639u+off)=1;
        if((terrain&4u)&&(sprite=B(0xadc36cu+tile))!=0){
            B(0xbcd66fu+off)=18;
            if(sprite<=6u)B(0xbcd671u+off)=bridge[sprite-1u];
            else if(sprite==13u)B(0xbcd671u+off)=30;
            else if(sprite==7u||sprite==8u||sprite==9u||sprite==10u)B(0xbcd671u+off)=14;
            else B(0xbcd671u+off)=36;
        }
    }
    kind=B(0xbcd602u+off);
    if(kind!=77u&&kind!=78u&&kind!=92u&&kind!=93u&&kind!=100u&&kind!=101u){
        tile=U(0xbcd618u+off);FUN_004b1210(actor,H(0xb350dcu+tile*2u));
    }
    W(0xbcd610u+off)=(uint16_t)old_x;W(0xbcd612u+off)=(uint16_t)old_y;
}
