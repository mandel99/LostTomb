#include "actor_walking.h"
#include "actor_routing.h"
#include "actor_order_flood.h"
#include "actor_order_path.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole4A5DA0 and its18-entry jump table. Slow cadence2 resets before
   stepping; the others reset/set/increment after the callback as specified. */
void FUN_004a5da0(int32_t actor,uint32_t speed)
{
    static const uint8_t limit[18]={0,2,1,0,1,2,0,2,1,0,1,2,0,2,1,0,1,2};
    uint32_t p=0xbcd758u+(uint32_t)actor*388u;int32_t steps,otherwise;
    if(speed>17u){FUN_004b27e0(actor,3);return;}
    if(!speed){FUN_004b27e0(actor,0);return;}
    if(speed==6u){FUN_004b27e0(actor,1);return;}
    if(speed==12u){FUN_004b27e0(actor,2);return;}
    if(speed==1u){
        if(B(p)<=2u)++B(p);else{FUN_004b27e0(actor,1);B(p)=0;}return;
    }
    if(speed==2u){
        if(B(p)<=1u)++B(p);else{B(p)=0;FUN_004b27e0(actor,1);}return;
    }
    if(speed==3u){
        if(!B(p)){FUN_004b27e0(actor,1);B(p)=1;}else B(p)=0;return;
    }
    if(speed==4u||speed==5u){
        if(B(p)<=limit[speed]){FUN_004b27e0(actor,1);++B(p);}else B(p)=0;return;
    }
    if(speed<=9u){steps=1;otherwise=2;}
    else if(speed<=11u){steps=2;otherwise=1;}
    else if(speed<=14u){steps=2;otherwise=3;}
    else{steps=3;otherwise=2;}
    if(B(p)<=limit[speed]){FUN_004b27e0(actor,steps);++B(p);}
    else{FUN_004b27e0(actor,otherwise);B(p)=0;}
}

/* Whole4B27E0. The explicit actor stays cached through all callbacks;
   progress/direction tests use signed BYTEs, route counters signed WORDs. */
void FUN_004b27e0(int32_t actor,int32_t steps)
{
    uint32_t off=(uint32_t)actor*388u,remaining=(uint32_t)steps;uint8_t phase;
    if(steps>0)do{
        --remaining;phase=(uint8_t)(B(0xbcd631u+off)+1u);B(0xbcd631u+off)=phase;
        if((int8_t)phase>=15){
            (void)FUN_004baa10(actor);B(0xbcd631u+off)=15;
            if(H(0xbcd632u+off)<=0)FUN_004b2d20(actor);
            FUN_004b33d0(actor);FUN_004b3530(actor);
            if((int8_t)B(0xbcd609u+off)>=8)break;
            ++W(0xbcd634u+off);B(0xbcd60au+off)=B(0xbcd609u+off);B(0xbcd631u+off)=0;
        }
        if(B(0xbcd631u+off)==8u)FUN_004b3c60(actor);
        FUN_004b3f00(actor);
    }while(I(remaining)>0);
    U(0xbcd5e8u)=0;
}

/* All path queries deliberately reload the cached actor's coordinates at
   each call. A previous callback may have changed any of those four WORDs. */
#define COORDS H(0xbcd60cu+off),H(0xbcd60eu+off),H(0xbcd61cu+off),H(0xbcd61eu+off)

/* Whole4B2D20. The original reuses its incoming actor argument as the
   route-slot out value: preserve that real prestate, not an invented zero. */
void FUN_004b2d20(int32_t actor)
{
    uint32_t off=(uint32_t)actor*388u,result=0,terrain;int32_t slot=actor;uint8_t water;
    int build_four=0;
    W(0xbcd632u+off)=0;W(0xbcd634u+off)=0;W(0xbcd636u+off)=0;
    if(!FUN_00489970(actor,&slot))return;
    water=B(0xbcd66eu+off);
    if(water){
        if(U(0x605a3cu)==1u)(void)FUN_0051d160(H(0x784c8cu),H(0x784c8eu));
        else if(water==2u)(void)FUN_0051d160(H(0xbcd60cu+off),H(0xbcd60eu+off));
        else(void)FUN_0051ce90(H(0xbcd60cu+off),H(0xbcd60eu+off));
        if(B(0xbcd66eu+off)==2u)result=(uint32_t)FUN_0051dc10(slot,COORDS,1);
        else if(B(0xbcd602u+off)==78u&&(B(0xbcd630u+off)==9u||U(0xbcd754u+off)==9u)){
            result=(uint32_t)FUN_0051d980(slot,COORDS,I(U(0xbcd738u+off)));
            if(!result)result=(uint32_t)FUN_0051dc10(slot,COORDS,0);
        }else result=(uint32_t)FUN_0051dc10(slot,COORDS,0);
    }else{
        terrain=B(0xbcd66au+off);
        switch(terrain){
        case 2:
            result=FUN_0051a420(COORDS,0,100000);
            if(!result)result=FUN_0051b040(COORDS);break;
        case 4:result=FUN_0051a030(COORDS);build_four=1;break;
        case 5:result=FUN_005181c0(COORDS);break;
        case 3:
            result=FUN_00519840(COORDS);
            if(result)build_four=1;else result=FUN_00518880(COORDS);break;
        case 6:
            result=FUN_00519be0(COORDS);if(!result)result=FUN_00517f20(COORDS);break;
        case 1:result=FUN_00519840(COORDS);build_four=1;break;
        case 0:result=FUN_00517f20(COORDS);break;
        case 9:result=FUN_00518400(COORDS);break;
        case 10:result=FUN_0051d420(COORDS);break;
        case 7:
            result=FUN_005195e0(COORDS);
            if(result)build_four=1;else result=FUN_00519390(COORDS);break;
        case 11:result=FUN_005186d0(COORDS);break;
        default:return;
        }
        if(!result)return;
        if(build_four){
            result=(uint32_t)FUN_0051b400(4,slot,COORDS);
            if(I(result)<=0)result=(uint32_t)FUN_0051b400(8,slot,COORDS);
        }else result=(uint32_t)FUN_0051b400(8,slot,COORDS);
    }
    if(result){W(0xbcd632u+off)=(uint16_t)slot;W(0xbcd636u+off)=(uint16_t)result;W(0xd04e58u+(uint32_t)slot*2u)=(uint16_t)actor;}
}
#undef COORDS
