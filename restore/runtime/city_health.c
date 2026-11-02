#include "population_updates.h"
#include "disease_updates.h"
#include "building_quality_update.h"
#include "actor_predicate.h"
#include "remaining_menu_actions.h"
#include "menu_full_input_numeric.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern int32_t FUN_004cde40(int32_t,int32_t);
extern int32_t FUN_004cde60(int32_t,int32_t);

static uint32_t city(void){return U(0xeaac28u)*0x49d8u;}

/* Match FILD/FMUL(single constant)/FILD/FMULP before the original FTOL.
   Keep inherited x87 PC/RC and caller stack values; no double spill. The
   shared raw80 truncation preserves the logical low32-bit integer result.
   FTOL exception/status flags are not reproduced by software truncation. */
static uint32_t weighted_health(int32_t score,int32_t people)
{
    uint8_t raw[10];
    __asm { fild score }
    __asm { mov eax,0x56f7d4 }
    __asm { fmul dword ptr [eax] }
    __asm { fild people }
    __asm { fmulp st(1),st(0) }
    __asm { lea eax,raw }
    __asm { fstp tbyte ptr [eax] }
    return (uint32_t)restore_menu_input_trunc80(raw);
}

/* Whole4E81D0. Both fixed scans visit IDs1..3999, independently of the
   live high-water count used by the later disease-latency provider. */
void FUN_004e81d0(void)
{
    uint32_t off=city(),total=0,weighted=0,id,p,score,food,value,bonus,index,owner;
    uint32_t previous,current,target,message;uint8_t skip=0,apothecary;
    U(0xea5fc0u+off)=0;
    if((int32_t)(int8_t)B(0xe38ea8u)<2&&!U(0xe38ebcu)){
        U(0xea1888u+off)=75;U(0xea1884u+off)=75;skip=1;
    }else{
        for(id=1,p=0x799ae0u;p<0x89b6d8u;++id,p+=264u){
            if(B(p)!=1||!B(p+5u)||!H(p+28u))continue;
            total+=(uint32_t)H(p+28u);
            restore_building_quality_004e7f00(I(id),B(0xeaac2cu));
            if(U(p+232u)==1000u){
                (void)FUN_00430060(I(id));off=city();++U(0xea5fc0u+off);
            }else if(U(p+252u)==1000u)FUN_00430040(I(id));
            score=B(p+120u)?25u:0u;apothecary=B(p+110u);
            if(apothecary)score+=15u;
            food=B(p+127u);if(food)score+=10u;if(food>1u)score+=5u;if(food>2u)score+=5u;
            if(food&&apothecary)score+=40u;
            weighted+=weighted_health(I(score),H(p+28u));
        }
        B(0xeaac2cu)=0;value=(uint32_t)FUN_004cde60(I(weighted),I(total));off=city();
        U(0xea1884u+off)=value;bonus=0;
        for(index=0;index<10u;++index)if(I(U(0xea1894u+off))<I(U(0x5e4b8cu+index*8u))){
            bonus=U(0x5e4b90u+index*8u);break;
        }
        U(0xea1884u+off)=value+bonus;
        if(W(0xea5ce8u+off)&&FUN_004c5120(I(U(0xea5f9cu+off)))==4){
            off=city();
            if((uint8_t)FUN_004c52d0(I(U(0xea5f9cu+off)),1)){
                off=city();value=(uint32_t)FUN_004cde40(I(U(0xea1884u+off)),105);
                off=city();U(0xea1884u+off)=value;
            }
        }
        if((uint8_t)FUN_0042fbb0())(void)FUN_0042fcf0(0);
        owner=U(0xeaac28u);off=owner*0x49d8u;previous=U(0xea1888u+off);target=U(0xea1884u+off);message=0;
        if(I(previous)<I(target)){
            current=previous+2u;U(0xea1888u+off)=current;target=U(0xea1884u+off);
            if(I(current)>I(target))U(0xea1888u+off)=target;
            current=U(0xea1888u+off);
            if(I(previous)/10!=I(current)/10&&I(U(0xea1894u+off))>300&&I(current)>=10){
                if(I(current)<20)message=0x63;else if(I(current)<30)message=0x64;else if(I(current)<40)message=0x65;
            }
        }else if(I(previous)>I(target)){
            current=previous-2u;U(0xea1888u+off)=current;target=U(0xea1884u+off);
            if(I(current)<I(target))U(0xea1888u+off)=target;
            current=U(0xea1888u+off);
            if((I(previous)/10!=I(current)/10&&I(U(0xea1894u+off))>300)||I(current)<20){
                if(I(current)<10)message=0x5f;else if(I(current)<20)message=0x60;
                else if(I(current)<30)message=0x61;else if(I(current)<40)message=0x62;
            }
        }
        if(message){FUN_004e2970(I(message));owner=U(0xeaac28u);}
        off=owner*0x49d8u;
        if(I(U(0xea1888u+off))>100)U(0xea1888u+off)=100;
        if(I(U(0xea1888u+off))<0)U(0xea1888u+off)=0;
    }
    for(p=0x799ae0u;p<0x89b6d8u;p+=264u)if(B(p)==1&&B(p+5u)&&B(p+135u)){
        value=U(p+240u);if(value){U(p+240u)=--value;if(!value)B(p+135u)=0;}
    }
    if(!skip){FUN_00430250();FUN_004302b0();}
}
