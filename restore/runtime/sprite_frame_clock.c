#include "sprite_frame_clock.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(a))

void restore_sprite_frame_clock_update(void)
{
    uint32_t now=(uint32_t)timeGetTime(),previous=U(0x12651a8u),enabled;
    uint16_t status;
    U(0x12651a4u)=now;
    U(0x12651a8u)=now;
    enabled=U(0x6044b4u);
    U(0x12651a0u)=now-previous;
    if(!enabled)return;
    if(((uint16_t)GetAsyncKeyState(VK_F2)&0x8001u)==0x8001u)
        U(0x6044b0u)=U(0x6044b0u)==0;
    if(((uint16_t)GetAsyncKeyState(VK_F5)&0x8001u)==0x8001u){
        /* Compare the extended intermediate, after storing binary32. This
           also retains the original unordered-value and rounding behavior. */
        __asm { mov eax,6044ach }
        __asm { fld dword ptr[eax] }
        __asm { mov edx,56f484h }
        __asm { fsub dword ptr[edx] }
        __asm { fst dword ptr[eax] }
        __asm { mov edx,56f6e4h }
        __asm { fcomp dword ptr[edx] }
        __asm { fnstsw status }
        if(status&0x100u)U(0x6044acu)=0x41200000u;
    }
    if(((uint16_t)GetAsyncKeyState(VK_F6)&0x8001u)==0x8001u){
        __asm { mov eax,6044ach }
        __asm { fld dword ptr[eax] }
        __asm { mov edx,56f484h }
        __asm { fadd dword ptr[edx] }
        __asm { fst dword ptr[eax] }
        __asm { mov edx,56f6e0h }
        __asm { fcomp dword ptr[edx] }
        __asm { fnstsw status }
        if(!(status&0x4100u))U(0x6044acu)=0x42a00000u;
    }
}
