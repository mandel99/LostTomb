#include "high_scores_menu.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
extern void FUN_004cc770(const char *,int,int,int,int);
extern void FUN_004ccbb0(int,int,int,int,int,int);
extern void FUN_004cce00(uint32_t,char,const char *,int,int,int,int);
void FUN_004c54c0(void)
{
    int32_t row=0,pixels=0;
    if(!I(0xd3a4bc)) {
        FUN_004ccbb0(296,6,I(0xe38da8)+48,I(0xe38da4)+104,I(0xf56f94),0);
        return;
    }
    while(row<I(0x700994)) {
        uint32_t index=(uint32_t)I(0xd3a4b8)+(uint32_t)row;
        const RestoreHighScore *score=(const RestoreHighScore *)(uintptr_t)(0xd38708u+index*76u);
        const char *mission=(const char *)(uintptr_t)(0x6ff60cu+index*50u);
        if(score->valid&&mission) {
            FUN_004cc770(mission,I(0xe38da8)+48,I(0xe38da4)+104+pixels,I(0xf56f84),0);
            I(0xe20778)=0;
            FUN_004ccbb0(296,5,I(0xe38da8)+224,I(0xe38da4)+104+pixels,I(0xf56f84),0);
            FUN_004cc770(score->family,I(0xe20778)+224+I(0xe38da8),I(0xe38da4)+104+pixels,I(0xf56f84),0);
            FUN_004ccbb0(153,(int32_t)((uint32_t)score->difficulty+1u),I(0xe38da8)+368,I(0xe38da4)+104+pixels,I(0xf56f84),0);
            FUN_004cce00(score->score,64,(const char *)0x57c048u,I(0xe38da8)+512,I(0xe38da4)+104+pixels,I(0xf56f84),0);
            pixels+=16;
        }
        if(pixels>=320)return;
        ++row;
    }
}
