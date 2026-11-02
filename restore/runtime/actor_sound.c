#include "actor_constable.h"
#include "actor_coordinates.h"
#include "menu_full_input_numeric.h"
#include "sound_initialization.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))

/* Native53B4A0/100 instructions. Exact16-case lowering of the immutable
   91-byte remap/two-target table53B614/53B60C; no executable token reads. */
static int offscreen_sound(uint8_t kind)
{
    switch(kind){
    case 11:case 12:case 13:case 43:case 44:case 45:case 54:case 55:
    case 56:case 77:case 78:case 92:case 93:case 99:case 100:case 101:return 1;
    default:return 0;
    }
}
static uint32_t pan_position(int32_t width,int32_t x)
{
    const float span=128.0f;uint8_t raw80[10];
    /* Preserve the native FILD/FDIVR/FILD/FMUL order and inherited x87 PC/RC.
       Spill extended precision for the genuine truncation primitive; never
       round the intermediate ratio or product through C float/double. */
    __asm {
        fild width
        fdivr dword ptr span
        fild x
        fmul st(0),st(1)
        lea eax,raw80
        fstp tbyte ptr[eax]
        fstp st(0)
    }
    return (uint32_t)restore_menu_input_trunc80(raw80);
}
void FUN_0053b4a0(int32_t actor,int32_t sound,int32_t variant)
{
    uint32_t record,width,sample;int32_t x,y,volume;int quiet=0;
    if(!U(0xf55b88u)||sound>=33||variant>=5||!B(0xe38e63u))return;
    width=U(U(0xf53f78u)?0xf53f60u:0xf53f64u);
    record=0xbcd600u+(uint32_t)actor*388u;
    FUN_004f0ba0(S(record+12u),S(record+14u),&x,&y);
    if(x<0||y<0||x>(int32_t)width||y>(int32_t)U(0xe38d98u)){
        if(!offscreen_sound(B(record+2u)))return;
        quiet=1;
    }
    sample=U(0xf54120u+((uint32_t)sound*5u+(uint32_t)variant)*4u);
    if((int32_t)sample<=-1)return;
    U(0xe92db4u)=pan_position((int32_t)U(U(0xf53f78u)?0xf53f60u:0xf53f64u),x);
    volume=(int32_t)U(0xe38ec4u);
    U(0xe92db0u)=(uint32_t)(quiet?volume/4:volume);
    U(0xe92dacu)=sample;U(0xe92da8u)=UINT32_MAX;U(0xe92db8u)=1;
    (void)FUN_0055f0e8(RESTORE_SOUND,(const int32_t *)(uintptr_t)0xe92da8u);
}
