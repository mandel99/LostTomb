#include "sprite_render.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))

/* Whole 40FF50. Native row/column counters test for zero, not positivity;
   cached byte width and each row's actual source/destination skips differ. */
void FUN_0040ff50(const void *pixels)
{
    uint32_t source=(uint32_t)(uintptr_t)pixels;
    uint32_t destination=U(0xe92e18u)+U(0xe38d94u)*U(0xd3b5dcu)+U(0xd3b5e0u)*2u;
    uint32_t width_bytes=U(0xd3b544u)*2u,rows=U(0xd3b5d4u),columns,skip;
    uint16_t pixel;
    if(rows==0)return;
    do{
        columns=U(0xd3b544u);destination+=width_bytes;
        if(columns!=0){
            do{
                pixel=W(source);
                if(pixel!=0xf81fu)W(destination)=pixel;
                destination-=2u;source+=2u;
            }while(--columns!=0);
        }
        skip=U(0xd3b53cu);source+=U(0xd3b540u);
        destination+=skip+width_bytes;
    }while(--rows!=0);
}
