#include "god_remaining_commands.h"
#include "menu_event_actions.h"
#include "god_effects.h"
#include "event_publisher.h"
#include "actor_create.h"
#include "actor_walking.h"
#include "command_migration.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* These five complete command-table callbacks contain9/11/9/11/18 native
   instructions. All five explicitly PUSH0 for the event parameters pointer:
   no decoder-stack bytes, synthesized payload, or default padding is used. */
static void publish(int32_t message)
{
    U(0xe950dcu)=1;FUN_004e0cf0(message,0,0,0,0);
}
void FUN_0041cbc0(void){publish(0x1e3);restore_god_city_004e6eb0();}
void FUN_0041cbf0(void){publish(0x1e4);restore_god_city_004e70b0(6);}
void FUN_0041cc20(void){publish(0x1e5);restore_god_city_004e6ed0();}
void FUN_0041cc50(void){publish(0x1e6);restore_god_command_004e7200(5);}
void FUN_0041ccb0(void)
{
    if(U(0x605a3cu)!=1){publish(0x1f0);restore_god_command_004e7ac0(10);}
    B(0xea137cu)=1;
}
int restore_god_remaining_command_action(uint32_t token)
{
    switch(token){
    case 0x41cbc0u:FUN_0041cbc0();break;
    case 0x41cbf0u:FUN_0041cbf0();break;
    case 0x41cc20u:FUN_0041cc20();break;
    case 0x41cc50u:FUN_0041cc50();break;
    case 0x41ccb0u:FUN_0041ccb0();break;
    default:return 0;
    }
    return 1;
}

/* Exactly the native CDQ/IDIV for a positive5000 numerator, with the
   incremented signed-DWORD divisor. Retains native #DE for a zero divisor;
   it does not introduce a C division-by-zero fallback or signed overflow. */
static uint32_t divide_5000(uint32_t divisor)
{
    uint32_t quotient;
    __asm {
        mov eax,5000
        cdq
        idiv divisor
        mov quotient,eax
    }
    return quotient;
}
/* Complete4E7200,90 instructions. */
void restore_god_command_004e7200(int32_t duration)
{
    uint32_t count,cursor,value,limit,actor=1;uint8_t kind,state;
    if(duration<3)duration=3;else if(duration>9)duration=9;
    count=U(0x78d790u);
    if((int32_t)count>=1){
        cursor=0x799bc8u;
        do {
            if(B(cursor-227u)){
                value=divide_5000(U(cursor+12u)+1u);
                U(cursor)=U(cursor)+value;
                value=divide_5000(U(cursor+12u)+1u);
                U(cursor+20u)=U(cursor+20u)+value;
                limit=U(cursor+12u)*3u;
                value=B(cursor-113u);
                if((int32_t)limit<(int32_t)value)B(cursor-113u)=(uint8_t)limit;
            }
            cursor+=264u;--count;
        }while(count);
    }
    for(cursor=0xbcd786u;cursor<0xc8ad42u;cursor+=388u,++actor){
        if(!B(cursor+4u))continue;
        kind=B(cursor);
        if(kind==95)FUN_004b5420((int32_t)actor);
        else if(kind==25){
            state=B(cursor+46u);
            if(state==10||state==9){
                W(cursor+44u)=0;
                B(cursor+46u)=13;
                W(cursor+26u)=W(cursor+34u);
                W(cursor+28u)=W(cursor+36u);
                FUN_004b3250((int32_t)actor);
            }
        }
    }
    value=(uint32_t)duration*816u;
    U(0x78d7b0u)=value;
    U(0x78d7acu)=value;
    restore_god_city_004e8e80(-15);
}

/* Reuse the canonical complete event-frame body. Its actual return value is
   available to native-shaped callers; this command-table interface is void. */
void restore_god_command_004e7ac0(int32_t duration)
{
    (void)FUN_004e7ac0(duration);
}
