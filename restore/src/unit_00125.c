#include "pixel_access.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Whole40FAA0. Native MOV AX preserves the row product's upper WORD before
   applying the full mask and publishing the DWORD scratch. All five incoming
   calls consume EAX; MUL's incidental EDX is not a second return value. */
uint32_t FUN_0040faa0(int32_t x,uint32_t y,uint32_t mask)
{
    uint32_t base=U(0xe92e18u),pitch=U(0xe38d94u),row=y*pitch;
    uint32_t at=base+(uint32_t)x*2u+row;
    uint32_t value=(row&0xffff0000u)|*(volatile uint16_t *)(uintptr_t)at;
    U(0x605b70u)=value&mask;
    return U(0x605b70u);
}
