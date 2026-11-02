#include "religion_updates.h"
#include "city_updates.h"
#include "god_effects.h"
#include <stdbool.h>
extern uint32_t FUN_00562348(void);
extern void FUN_00417ae0(int32_t instruction);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)(int8_t)B(a))
#define I(v) ((int32_t)(uint32_t)(v))
static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static int active(uint32_t god){return W(0x7848e2u+god*2u)!=0;}

/* Whole4E9F50: strict comparisons retain the first tie. Positive wrath
   takes priority; otherwise choose a mood below40. Result is full EAX0/1. */
uint32_t FUN_004e9f50(void)
{
    uint32_t offset=player_offset(),i,selected=0;int32_t best=0,level;
    for(i=0;i<5u;++i)if(active(i)){
        level=S(0xea5c14u+offset+i);
        if(level>best){best=level;selected=i+1u;}
    }
    if(!selected){
        best=40;
        for(i=0;i<5u;++i)if(active(i)){
            level=S(0xea5c0au+offset+i);
            if(level<best){best=level;selected=i+1u;}
        }
    }
    U(0xea5c78u+offset)=selected;
    return selected!=0;
}

/* Whole4C51D0: nonnegative WORD/DWORD timers expire at-1, not at0. */
void FUN_004c51d0(void)
{
    uint32_t offset=player_offset(),i;
    for(i=0;i<3u;++i)if(H(0xea61f4u+offset+i*2u)>=0)
        W(0xea61f4u+offset+i*2u)=(uint16_t)(W(0xea61f4u+offset+i*2u)-1u);
    if(I(U(0xea5e3cu+offset))>=0)--U(0xea5e3cu+offset);
    if(I(U(0xea6024u+offset))>=0)--U(0xea6024u+offset);
}

/* Whole4E9890, including both preview and monthly paths. Mood/accumulator
   comparisons are signed BYTEs; availability is WORD and player/counts are
   DWORD. Keep cached offsets across the corresponding native calls. */
void FUN_004e9890(int32_t advance)
{
    uint32_t offset,i,at,table,player;int32_t population,minimum,maximum,elapsed;
    int32_t selected,level,mood,best;uint8_t cached_flag;
    if(!U(0xe38ebcu)&&!B(0x7027bcu)){
        offset=player_offset();
        for(i=0;i<5u;++i){B(0xea5c00u+offset+i)=30;B(0xea5c0au+offset+i)=30;}
        return;
    }
    offset=player_offset();
    for(i=0;i<5u;++i)if(active(i))B(0xea5c00u+offset+i)=B(0x78d620u+i*4u);
    for(i=0;i<5u;++i)if(active(i)){
        elapsed=I(U(0xea5c50u+offset+i*4u));if(elapsed>40)elapsed=40;
        B(0xea5c00u+offset+i)=(uint8_t)(B(0xea5c00u+offset+i)+12u-(uint32_t)elapsed);
    }
    population=I(U(0xea1894u+offset));minimum=50;maximum=50;
    if(population>=600){minimum=0;maximum=100;}
    else if(population>=550){minimum=10;maximum=90;}
    else if(population>=500){minimum=20;maximum=80;}
    else if(population>=450){minimum=30;maximum=70;}
    else if(population>=400){minimum=40;maximum=60;}
    for(i=0;i<5u;++i)if(active(i)){
        at=0xea5c00u+offset+i;
        if(S(at)>100)B(at)=100;
        if(S(at)<minimum)B(at)=(uint8_t)minimum;
        if(S(at)>maximum)B(at)=(uint8_t)maximum;
    }
    if(!advance)return;
    cached_flag=B(0xe38ea9u);player=U(0xe38ebcu);
    for(i=0;i<5u;++i)if(active(i)){
        at=0xea5c0au+offset+i;mood=S(0xea5c00u+offset+i);
        if(S(at)>mood)--B(at);else if(S(at)<mood)++B(at);
        if(!B(0xe38ea8u)&&!player&&!cached_flag&&S(at)<50)B(at)=50;
    }
    selected=I(FUN_00562348())%5;offset=player_offset();
    if(selected<=4&&active((uint32_t)selected)){
        at=offset+(uint32_t)selected;level=S(0xea5c0au+at);
        if(level>50){
            table=level>=90?0x56f720u:level>=80?0x56f734u:level>=70?0x56f748u:0x56f75cu;
            B(0xea6086u+at)=(uint8_t)(B(0xea6086u+at)+B(table+U(0xe39080u)*4u));
            B(0xea5c14u+at)=0;
        }else if(level==50)B(0xea5c14u+at)=0;
        else{
            table=level>30?0x56f784u:level>20?0x56f798u:level>10?0x56f7acu:0x56f7c0u;
            B(0xea6086u+at)=0;
            B(0xea5c14u+at)=(uint8_t)(B(0xea5c14u+at)+B(table+U(0xe39080u)*4u));
        }
        if(S(0xea5c14u+at)>=50)B(0xea5c14u+at)=50;
        if(S(0xea6086u+at)>=50)B(0xea6086u+at)=50;
    }
    for(i=0;i<5u;++i){
        if(S(0xea5c0au+offset+i)<50)B(0xea6086u+offset+i)=0;
        if(S(0xea5c0au+offset+i)>50)B(0xea5c14u+offset+i)=0;
    }
    if(!U(0xe38ebcu)&&S(0xe38ea8u)==I(U(0x57953cu))&&!B(0x7027bdu)&&
       B(0x7027bcu)&&S(0xea5c0eu+offset)>=40)FUN_00417ae0(5);
    if(U(0x78d7c4u))return;
    FUN_004c51d0();offset=player_offset();
    if(!W(0xea5ce8u+offset)){
        for(i=0;i<5u;++i)if(H(0x7848e2u+i*2u)==2)
            B(0xea5c00u+offset+i)=(uint8_t)((S(0xea5c00u+offset+i)*98+50)/100);
    }else for(i=0;i<5u;++i)if(active(i)){
        uint32_t kind;int32_t factor;
        offset=player_offset();kind=FUN_004c06e0(i,1);
        if((uint32_t)H(0x7999e8u+U(0xea5f9cu+offset)*264u)==kind)factor=115;
        else if(H(0x7848e2u+i*2u)==2)factor=90;
        else if(H(0x7848e2u+i*2u)==1)factor=95;
        else continue;
        B(0xea5c00u+offset+i)=(uint8_t)((S(0xea5c00u+offset+i)*factor+50)/100);
    }
    for(i=0;i<5u;++i)if(active(i)){
        at=0xea5c00u+player_offset()+i;
        if(S(at)>100)B(at)=100;
        if(S(at)<minimum)B(at)=(uint8_t)minimum;
    }
    offset=player_offset();
    for(i=0;i<5u;++i)if(active(i))++U(0xea5c50u+offset+i*4u);
    /* The original fallback is retained. A signed remainder modulo5 is
       always<=4, so a conforming RNG/callee cannot take this branch. */
    if(selected>4&&FUN_004e9f50())selected=I(U(0xea5c78u+player_offset())-1u);
    if(!U(0xe39084u))return;
    /* Native PUSH-1 before the nullary RNG belongs to argument3 of the
       later effect call. Source/choice is unused by the original effect. */
    restore_god_effect_004c47a0(selected,(uint32_t)(I(FUN_00562348())%3),-1);
    offset=player_offset();best=100;
    for(i=0;i<5u;++i)if(active(i)&&S(0xea5c0au+offset+i)<best)best=S(0xea5c0au+offset+i);
    if(U(0xea5e48u+offset))--U(0xea5e48u+offset);
    else if(best<30)U(0xea5e48u+offset)=20;
}
