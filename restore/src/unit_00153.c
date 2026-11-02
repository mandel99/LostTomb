#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* Whole00411fd0: opaque 32-by-32 pixels. Each native DWORD load is followed
   immediately by its store; forward overlap and the cached pitch matter. */
void FUN_00411fd0(const void *pixels)
{
    uint32_t source=(uint32_t)(uintptr_t)pixels;
    uint32_t destination=U(0xe92e18u),pitch,row,column,value;
    destination+=U(0xd3b5e0u)*2u;
    destination+=U(0xd3b5dcu)*U(0xe38d94u);
    pitch=U(0xe38d94u);
    for(row=0;row<32u;++row){
        for(column=0;column<16u;++column){
            value=U(source);U(destination+column*4u)=value;source+=4u;
        }
        destination+=pitch;
    }
}
