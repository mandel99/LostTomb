#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))

/* Whole410F70. Copy two pixels, then reread the second source pixel for
   the first pixel of the next row. The native shape contains three pixels. */
void FUN_00410f70(const void *pixels)
{
    uint32_t source=(uint32_t)(uintptr_t)pixels;
    uint32_t destination=U(0xe92e18u),pitch,value;
    destination+=U(0xd3b5e0u)*2u;
    destination+=U(0xd3b5dcu)*U(0xe38d94u);
    pitch=U(0xe38d94u);
    value=U(source);U(destination)=value;
    W(destination+pitch)=W(source+2u);
}
