#include "high_scores_menu.h"
#include "sprite_render.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
extern int FUN_004cde40(int,int),FUN_004cde60(int,int);
void FUN_004c5450(void)
{
    int32_t total=I(0xd3a4bc),first=I(0xd3a4b8),percent,y;
    if((int32_t)((uint32_t)total-20u)<=0)return;
    if(first<=0)percent=0;
    else if((int32_t)((uint32_t)first+20u)<total)percent=FUN_004cde60(first,total-20);
    else percent=100;
    y=FUN_004cde40(257,percent);
    FUN_0053e590(I(0xf56494)+39,I(0xe38da8)+584,I(0xe38da4)+130+y);
}
