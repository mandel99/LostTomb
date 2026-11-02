#include "menu_timers.h"
#include "video_open.h"
#include "event_publisher.h"
#include "game_diagnostics.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern int32_t FUN_004ea280(int32_t minimum_age);
extern uint32_t FUN_004ea930(void);
extern uint32_t FUN_004f52f0(int32_t slot);
extern void FUN_0051f780(void),FUN_004cf1c0(void),FUN_004ea920(void);

/* Whole4EA2C0. Mission goals and city fields are full DWORDs at byte stride
   49D8. Only actual flags/modes are BYTEs. Cache the native player identifier
   until its explicit reload points, including after outcome callbacks. */
void FUN_004ea2c0(void)
{
    uint32_t player,offset,goals=0,deadline,time_goal,forces,threshold,slot;
    uint32_t population,value,first,presentation;
    int32_t x,y;
    const char *movie;
    B(0xea17fcu)=0;
    if(W(0x784b60u))return;
    player=U(0xeaac28u);B(0xe38dd9u)=1;
    if(B(0x784c3cu)){
        ++goals;if(I(U(0xea5b3cu+player*0x49d8u))<I(U(0x784c24u)))B(0xe38dd9u)=0;
    }
    if(B(0x784c3du)){
        ++goals;if(I(U(0xea5b40u+player*0x49d8u))<I(U(0x784c28u)))B(0xe38dd9u)=0;
    }
    if(U(0x784c58u)){
        ++goals;if(I(U(0xea1894u+player*0x49d8u))<I(U(0x784c5cu)))B(0xe38dd9u)=0;
    }
    if(B(0x784c3eu)){
        ++goals;if(I(U(0xea5b44u+player*0x49d8u))<I(U(0x784c2cu)))B(0xe38dd9u)=0;
        if(!(uint8_t)FUN_004ea930())B(0xe38dd9u)=0;
        for(slot=0;slot<3u;++slot)(void)FUN_004f52f0(I(slot));
        player=U(0xeaac28u);
    }
    if(B(0x784c3fu)){
        ++goals;if(I(U(0xea5b48u+player*0x49d8u))<I(U(0x784c30u)))B(0xe38dd9u)=0;
    }
    if(U(0x784c34u)){
        int32_t houses;
        ++goals;houses=FUN_004ea280(I(U(0x784c38u)));
        if(houses<I(U(0x784c34u)))B(0xe38dd9u)=0;
        player=U(0xeaac28u);
    }
    if(!U(0xe38ebcu)&&(int32_t)(int8_t)B(0xe38ea8u)==I(U(0x5795e4u))&&
       !B(0x7027c3u)&&B(0x7027c2u)){
        if(FUN_004ea280(9)>0)FUN_00417ae0(11);
        player=U(0xeaac28u);
    }
    deadline=U(0x784c48u);time_goal=U(0x784c50u);
    if(I(goals)<=0){
        B(0xe38dd9u)=0;
        if(deadline)++goals;if(time_goal)++goals;
    }else if(B(0xe38dd9u)==1u&&!time_goal)goto military;
    threshold=U(0x78d7b8u);
    if(deadline){
        value=U(0xea6018u+player*0x49d8u);
        if(!value)threshold+=7u;else if(value==1u)threshold+=2u;
    }
    if(I(U(0x78d7bcu))>=I(threshold)){
        if(deadline||(time_goal&&!B(0xe38dd9u)))B(0xe38dd9u)=255;
    }else if(time_goal)B(0xe38dd9u)=0;
military:
    offset=player*0x49d8u;forces=U(0xea5eb4u+offset)+U(0xea5ea4u+offset);
    population=U(0xea1894u+offset);
    if(I(forces)>I(U(0xea5ee8u+offset)+2u)&&I(population)<I(U(0xea5f0cu+offset))/4){
        B(0xe38dd9u)=255;B(0xea17fcu)=1;
    }
    if(I(forces)>0&&I(U(0xea1894u+offset))<=0){B(0xe38dd9u)=255;B(0xea17fcu)=1;}
    if(U(0x605a3cu)==1u||I(goals)<=0)B(0xe38dd9u)=0;
    if(U(0xea5d40u+offset))B(0xe38dd9u)=I(U(0xea5d44u+offset))<=0;
    if(B(0xe38dd8u))B(0xe38dd9u)=1;else if(!B(0xe38dd9u))return;
    U(0x6059e8u)=0;B(0xe3908cu)=0;
    restore_game_trace("mission outcome player=%u scene=%d result=%d goals=%u\n",
        player,(int32_t)(int8_t)B(0xe38ea8u),(int32_t)(int8_t)B(0xe38dd9u),goals);
    if((int8_t)B(0xe38dd9u)>=1){
        FUN_0051f780();offset=U(0xeaac28u)*0x49d8u;
        first=U(0xea5f88u+offset);
        if(!first){U(0xea5f88u+offset)=1;U(0xe950dcu)=1;B(0xe38ddau)=67;B(0xe38dcau)=1;return;}
        B(0xe38e07u)=0;B(0xe38df8u)=0;B(0xe38e04u)=0;U(0xf54118u)=2;
        FUN_004cf1c0();
        x=I(U(0xe38db0u)-400u)/2;y=I(U(0xe38dacu)-292u)/2;
        if((int8_t)B(0xe38ea8u)>=10&&!U(0xe38ebcu)){
            (void)FUN_00538dc0((const char *)(uintptr_t)0x5e4c0cu,(uint32_t)x,(uint32_t)y,1,66);
            B(0xe38dd8u)=0;return;
        }
        presentation=U(0xe3907cu);
        movie=(const char *)(uintptr_t)(presentation?0x5e4bdcu:0x5e4bf4u);
        (void)FUN_00538dc0(movie,(uint32_t)x,(uint32_t)y,1,66);
        B(0xe38dd8u)=0;U(0xe3907cu)=U(0xe3907cu)==0;return;
    }
    if(!U(0xea5f84u+offset)&&(B(0xea17fcu)||!deadline||I(U(0xea6018u+offset))<=0)){
        U(0xea5f84u+offset)=1;U(0xe950dcu)=1;
        FUN_004e0cf0(311,0,0,0,0);B(0xe38dd8u)=0;return;
    }
    FUN_004ea920();FUN_0041aa20(1,51,1000);B(0xe38dd8u)=0;
}
