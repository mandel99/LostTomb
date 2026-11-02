#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))

/* Whole4101C0. The clipped width, both row increments and framebuffer
   geometry are full DWORDs. F81F is transparent; opaque source pixels AND
   the existing destination with the low WORD mask. */
void FUN_004101c0(const void *pixels,uint16_t mask)
{
    uint32_t source=(uint32_t)(uintptr_t)pixels;
    uint32_t destination=U(0xe92e18u),rows,remaining;
    destination+=U(0xd3b5e0u)*2u;
    destination+=U(0xd3b5dcu)*U(0xe38d94u);
    rows=U(0xd3b5d4u);
    do{
        remaining=U(0xd3b544u);
        do{
            if(W(source)!=0xf81fu)W(destination)=(uint16_t)(W(destination)&mask);
            source+=2u;destination+=2u;--remaining;
        }while((int32_t)remaining>0);
        source+=U(0xd3b540u);
        destination+=U(0xd3b53cu);
        --rows;
    }while((int32_t)rows>0);
}
