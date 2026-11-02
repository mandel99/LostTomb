#include "financial_advisor_render.h"
#include "sprite_render.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* Whole536BA0: width1 draws the first block. Interior images cycle through
   five variants. Native Y scratch survives each sprite callback. */
void FUN_00536ba0(int32_t x,int32_t y,int32_t blocks)
{
    uint32_t index,variant=0,image,px;
    U(0xd3b5dcu)=(uint32_t)y;
    for(index=0;(int32_t)index<blocks;++index){
        px=(uint32_t)x+index*16u;U(0xd3b5e0u)=px;
        if(!index)image=U(0xf56844u)+42u;
        else if((int32_t)index<blocks-1){image=U(0xf56844u)+43u+variant;++variant;}
        else image=U(0xf56844u)+48u;
        U(0xd3b5c0u)=image;
        (void)FUN_0053e590((int32_t)image,(int32_t)px,(int32_t)U(0xd3b5dcu));
        if(variant>=5u)variant=0;
    }
}
