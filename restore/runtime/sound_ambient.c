#include "sound_ambient.h"
#include "sound_streams.h"
#include "sound_fades.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

static void industrial_comparison(void)
{
    /* Both native branch targets open the already-pushed industrial filename.
       Retain the calculation's x87 effects: inherited precision/rounding,
       exceptions and condition bits, and two pushes followed by two pops.
       MSVC double would add a rounding step and lose the original status. */
    __asm {
        mov eax,0f55b58h
        fild dword ptr[eax]
        mov eax,0f55b84h
        fild dword ptr[eax]
        mov eax,0570158h
        fmul qword ptr[eax]
        fcompp
    }
}

void restore_sound_update_ambient(void)
{
    int32_t best=0,selected=-1,weight,population;
    uint32_t i,path;
    if(!U(0xf55b88u)||!B(0xe38ec0u))return;
    for(i=0;i<12u;++i){
        weight=(int32_t)U(0xf55b50u+i*4u);
        if(weight>best){best=weight;selected=(int32_t)i;}
    }
    if((uint32_t)selected==U(0xf55b80u)&&FUN_0055f5a1(RESTORE_SOUND,2)){
        if((int32_t)FUN_0055f5f4(RESTORE_SOUND,2)<100)FUN_0053b790(2);
        return;
    }
    U(0xf55b80u)=(uint32_t)selected;
    switch(selected){
    case 1:
        population=(int32_t)U(0xea1894u+U(0xeaac28u)*0x49d8u);
        if(population<500)path=0x5fac08u;
        else if(population<2000)path=0x5fabecu;
        else if(population<5000)path=0x5fabd0u;
        else if(population<8000)path=0x5fabb4u;
        else path=0x5fab98u;
        break;
    case 2:industrial_comparison();path=0x5fac24u;break;
    case 3:case 7:path=0x5facb4u;break;
    case 4:
        /* Native signed DWORD threshold. The identity of5F9058 is unproved. */
        path=(int32_t)U(0x5f9058u)<1000?0x5fab78u:0x5fab58u;
        break;
    case 6:path=0x5facd0u;break;
    case 8:path=0x5fac94u;break;
    case 9:path=0x5fac7cu;break;
    case 10:path=0x5fac44u;break;
    case 11:path=0x5fac5cu;break;
    default:
        FUN_0055f561(RESTORE_SOUND,2);
        FUN_0055f639(RESTORE_SOUND,2,0);
        return;
    }
    (void)FUN_0055f449(RESTORE_SOUND,(const char *)(uintptr_t)path,2,0);
    FUN_0055f639(RESTORE_SOUND,2,0);
}

