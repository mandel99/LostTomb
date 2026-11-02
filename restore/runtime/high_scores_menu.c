#include "high_scores_menu.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern int FUN_00425990(int,int,const void *,int);
/* Both native small-button descriptors select this callback and pass their
   primary argument through 00d3b198. */
void FUN_004c5400(void)
{
    int32_t total=I(0xd3a4bc),action=I(0xd3b198),first=I(0xd3a4b8);
    if(total<=20)return;
    if(!action) {
        if(first<=0)return;
        I(0xd3a4b8)=(int32_t)((uint32_t)first-1u);B(0xe38dca)=2;return;
    }
    if(action==1) {
        if((int32_t)((uint32_t)first+20u)>=total)return;
        I(0xd3a4b8)=(int32_t)((uint32_t)first+1u);
    }
    B(0xe38dca)=2;
}
void restore_high_scores_input(void)
{
    if(B(0xe38df8)) {B(0xe38dda)=0;B(0xe38dca)=1;return;}
    if(FUN_004c57d0())return;
    FUN_00425990(I(0xe38da8),I(0xe38da4),(const void *)0x5e01d0u,2);
}
