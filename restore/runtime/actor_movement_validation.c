#include "actor_walking.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
#define OFF() ((uint32_t)actor*388u)

static void relocate(int32_t actor,int32_t *x,int32_t *y)
{
    uint32_t off;
    (void)FUN_004ad880(x,y);FUN_004b5420(actor);off=OFF();
    W(0xbcd60cu+off)=(uint16_t)*x;W(0xbcd60eu+off)=(uint16_t)*y;
    U(0xbcd618u+off)=U(0xbca2d8u)+(uint32_t)*y*228u+(uint32_t)*x;
    B(0xbcd631u+off)=15;FUN_004b52f0(actor);
}

/* Whole4B3530, including ferry queues and both recovery jump tables.
   The local actor is the original incoming argument slot passed to queue
   insertion. It stays live across that call instead of caching its value. */
void FUN_004b3530(int32_t actor)
{
    uint32_t off=OFF(),tile,terrain,masked,building_off,other,queue,variant,result,kind,mode;
    int32_t direction=(int8_t)B(0xbcd609u+off),building,x,y,amount,i;
    if(direction>=8||B(0xbcd66au+off)==11u)return;
    tile=U(0xbcd618u+off)+U(0x5f80fcu+(uint32_t)direction*4u);
    building=H(0xb4e6fcu+tile*2u);terrain=U(0xaf598cu+tile*4u);masked=terrain&0xeefec75fu;
    if(B(0xbcd66eu+off)){
        if(!(masked&4u))goto blocked;
        if((int8_t)B(0xed4140u+tile)>=0)return;
        goto blocked;
    }
    mode=B(0xbcd66au+off);
    if(mode==2u){
        amount=(int8_t)B(0xec7630u+tile);
        if(amount<=-1)goto blocked;
        if(amount==0||amount==5)return;
        if(amount==2&&(terrain&0x400u))return;
        B(0xbcd60bu+off)=(uint8_t)direction;B(0xbcd609u+off)=11;
        if(!(B(0x78d7ccu)&3u))FUN_004690f0(I(tile),amount<=2?10:amount==3?200:400);
        return;
    }
    if(mode==4u){if((int8_t)B(0xebab20u+tile)>-1)return;goto blocked;}
    variant=B(0xee0c50u+tile);
    if(mode==6u){if(variant!=255u||(terrain&0x10004u))return;goto blocked;}
    if(variant==1u||variant==3u){
        kind=B(0xbcd602u+off);
        if(kind==0u||(kind>3u&&kind!=94u)){
            x=H(0xbcd60cu+off);y=H(0xbcd60eu+off);relocate(actor,&x,&y);
            FUN_004b3250(actor);off=OFF();B(0xbcd609u+off)=9;
        }
    }
    if(masked&0x40000440u){
        if(!U(0xbcd5e8u)||!(masked&8u))return;
        kind=W(0x7999e8u+(uint32_t)building*264u);
        if(kind==58u||kind==170u||kind==171u||kind==200u||kind==201u||kind==202u||kind==138u)goto blocked;
        return;
    }
    if(masked&0xc000u)goto blocked;
    if(!(masked&8u)){if(masked&0x1004u)goto blocked;return;}
    if(U(0xaf598cu+tile*4u)&0x10000000u)return;
    building_off=(uint32_t)building*264u;kind=W(0x7999e8u+building_off);
    if(kind==56u||(kind==54u&&B(0xbcd66au+off)!=0u)||
       (kind==183u&&B(0xbcd66au+off)==7u)||kind==209u)return;
    if(kind!=136u||U(0xbcd5e8u))goto blocked;
    kind=B(0xbcd602u+off);
    switch(kind){
    case 1:case 2:case 3:case 94:return;
    case 4:case 9:case 11:case 12:case 13:case 15:case 16:case 17:case 18:case 19:
    case 38:case 39:case 42:case 73:case 75:case 79:case 80:case 81:case 85:case 90:case 105:case 108:break;
    default:goto blocked;
    }
    variant=B(0x799aa2u+building_off);other=U(0x799a9cu+building_off);
    result=FUN_004b3470(actor,building);
    if(result==2u){
        for(i=0;i<3;++i){
            FUN_004b33d0(actor);off=OFF();B(0xbcd60au+off)=B(0xbcd609u+off);
            ++W(0xbcd634u+off);FUN_004b3c60(actor);
            if((int8_t)B(0xbcd609u+OFF())>=8)return;
        }
        return;
    }
    if(result!=1u){off=OFF();goto blocked;}
    queue=0xeaaf68u+variant*224u;
    if(!restore_actor_walking_queue_full((const void *)(uintptr_t)queue)&&
       H(0x799a18u+building_off)>0&&H(0x799a18u+other*264u)>0){
        off=OFF();if(B(0xbcd630u+off)==7u)return;
        (void)restore_actor_walking_queue_push((void *)(uintptr_t)queue,&actor);
        off=OFF();B(0xbcd608u+off)=B(0xbcd630u+off);B(0xbcd630u+off)=7;
        FUN_004b3250(actor);
        other=(uint32_t)H(0x799a02u+building_off);W(0xbcd62eu+other*388u)=0;return;
    }
    FUN_004b3250(actor);off=OFF();x=H(0xbcd60cu+off);y=H(0xbcd60eu+off);
    relocate(actor,&x,&y);off=OFF();kind=B(0xbcd602u+off);
    switch(kind){
    case 4:case 9:case 15:case 16:case 17:case 18:case 19:case 38:case 75:
    case 79:case 80:case 81:case 85:case 90:case 105:B(0xbcd630u+off)=8;break;
    case 11:case 12:case 13:case 42:B(0xbcd606u+off)=2;break;
    case 39:B(0xbcd630u+off)=9;break;
    case 73:B(0xbcd630u+off)=14;break;
    default:break;
    }
    return;
blocked:
    B(0xbcd609u+off)=9;
}
#undef OFF
