#include "pending_message.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
int32_t FUN_004cbf30(const uint8_t *text,int32_t font)
{
    uint32_t width=0,i,glyph,image;int32_t space,spacing,count;uint8_t character;
    if((uint32_t)font==U(0xf56fb4u)){space=10;spacing=1;}
    else if((uint32_t)font==U(0xf56fbcu)){space=10;spacing=1;}
    else if((uint32_t)font==U(0xf56f84u)){space=4;spacing=1;}
    else{space=6;spacing=0;}
    count=(int32_t)U(0x126517cu);
    for(i=0;i<10000;++i){
        character=B((uintptr_t)text+i);if(!character)break;
        if(character==32){width+=(uint32_t)space;continue;}
        glyph=B(0x5e047cu+character);U(0xd3b5c0u)=glyph;
        if(!glyph)continue;
        image=(uint32_t)font+glyph-1u;if((int32_t)image>=count)image=0;
        width+=(uint32_t)((int32_t)W(0x10d6a0cu+image*64u)+spacing);
    }
    return(int32_t)width;
}
int32_t FUN_004cc000(char *text)
{
    uint32_t i;
    for(i=0;i<10000;++i)if(!B((uintptr_t)text+i))break;
    return(int32_t)i;
}
