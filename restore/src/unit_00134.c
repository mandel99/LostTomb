#include "sprite_render.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))

/* Whole 40FED0. Preserve the native first pixel at x+width, including
   its one-pixel bias, and the fresh WORD width/DWORD height reloads. */
void FUN_0040fed0(const void *pixels)
{
    uint32_t source=(uint32_t)(uintptr_t)pixels;
    uint32_t destination=U(0xe92e18u)+U(0xe38d94u)*U(0xd3b5dcu)+U(0xd3b5e0u)*2u;
    uint32_t rows=U(0xd3b5d4u),width=(uint32_t)(int32_t)(int16_t)W(0xd3b5beu);
    uint32_t width_bytes=width*2u,row=0,column;
    uint16_t pixel;
    if((int32_t)rows<=0)return;
    do{
        destination+=width_bytes;column=0;
        if((int32_t)width>0){
            do{
                pixel=W(source);
                if(pixel!=0xf81fu)W(destination)=pixel;
                width=(uint32_t)(int32_t)(int16_t)W(0xd3b5beu);
                destination-=2u;source+=2u;++column;
            }while((int32_t)column<(int32_t)width);
        }
        destination+=U(0xd3b53cu)+width_bytes;++row;
    }while((int32_t)row<(int32_t)U(0xd3b5d4u));
}
