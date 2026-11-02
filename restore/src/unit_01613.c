#include "high_scores_menu.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern int FUN_004cde40(int,int),FUN_004cde60(int,int);
int32_t FUN_004c57d0(void)
{
    int32_t y,percent;
    if(I(0xd3a4bc)<=20||!B(0xe38e00))return 0;
    if(I(0xe38e34)<I(0xe38da8)+568||I(0xe38e34)>I(0xe38da8)+600)return 0;
    if(I(0xe38e30)<I(0xe38da4)+128||I(0xe38e30)>I(0xe38da4)+401)return 0;
    y=I(0xe38e30)-I(0xe38da4)-128;if(y>257)y=257;
    percent=FUN_004cde60(y,257);
    I(0xd3a4b8)=FUN_004cde40(I(0xd3a4bc)-20,percent);
    B(0xe38dca)=2;return 1;
}
