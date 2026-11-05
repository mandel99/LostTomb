#include "city_migration_actions.h"
#include "command_migration.h"
#include "campaign_score_numeric.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Exact 469BB7..469BC9 arithmetic: signed FILD32 and FMUL binary32 under
   inherited x87 PC/RC. Raw80 retains the actual intermediate; the existing
   FTOL-compatible converter temporarily selects truncation and restores CW.
   There is no binary64 spill, substituted constant, clamp or default count. */
static uint32_t migration_count(uint32_t scene)
{
    uint8_t raw80[10];
    __asm {
        mov eax,scene
        imul eax,eax,49d8h
        fild dword ptr[eax+0ea1894h]
        mov eax,056f568h
        fmul dword ptr[eax]
        lea eax,raw80
        fstp tbyte ptr[eax]
    }
    return (uint32_t)restore_campaign_score_trunc80(raw80);
}

/* Whole469B90: the DWORD timer and all arithmetic wrap before native signed
   comparisons. The quota is only tested for zero, including negative FTOL
   results. The first three eligible candidates can request the alternate actor.
   Reload the count on every completed iteration and the kind after creation. */
void restore_city_update_migration_requests(void)
{
    uint32_t tick=U(0x978d80u)+1u,scene,remaining,count,index,cursor,alternatives=3u,kind,value;
    uint8_t alternative;
    U(0x978d80u)=tick;
    if((int32_t)tick<10)return;
    scene=U(0xeaac28u);
    U(0x978d80u)=0;
    remaining=migration_count(scene);
    count=U(0x78d790u);index=1u;cursor=0x799ae5u;
    if((int32_t)count<1)return;
    do {
        if(remaining==0)return;
        if(B(cursor-5u)==1u&&B(cursor)!=0&&W(cursor+0x17u)!=0&&U(cursor+0xdfu)==1000u){
            alternative=(uint8_t)FUN_00469d50();
            FUN_00469e40((int32_t)index,(uint8_t)(alternative!=0&&alternatives!=0));
            kind=(uint32_t)H(cursor+0xdu);
            --remaining;
            value=U(0x70ce0cu+kind*88u);
            U(cursor+0xdfu)=value;
            if((int32_t)alternatives>0)--alternatives;
        }
        count=U(0x78d790u);
        ++index;cursor+=264u;
    }while((int32_t)index<=(int32_t)count);
}
