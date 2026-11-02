#include "high_scores_menu.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_00536820(int,int,int,int),FUN_00536a30(int,int,int,int);
extern void FUN_004ccb30(int,int,int,int,int,int,int);
extern void FUN_004ccbb0(int,int,int,int,int,int);
void FUN_004c5660(void)
{
    FUN_00536820(I(0xe38da8),I(0xe38da4),40,30);
    FUN_00536a30(I(0xe38da8)+32,I(0xe38da4)+96,34,22);
    FUN_004ccb30(296,0,I(0xe38da8)+20,I(0xe38da4)+32,608,I(0xf56fb4),0);
    FUN_00536a30(I(0xe38da8)+579,I(0xe38da4)+123,2,19);
    FUN_004ccbb0(296,1,I(0xe38da8)+80,I(0xe38da4)+80,I(0xf56f8c),0);
    FUN_004ccbb0(296,2,I(0xe38da8)+224,I(0xe38da4)+80,I(0xf56f8c),0);
    FUN_004ccbb0(296,3,I(0xe38da8)+368,I(0xe38da4)+80,I(0xf56f8c),0);
    FUN_004ccbb0(296,4,I(0xe38da8)+512,I(0xe38da4)+80,I(0xf56f8c),0);
    FUN_004c54c0();FUN_004c5450();
    FUN_004ccb30(13,1,I(0xe38da8)+20,I(0xe38da4)+452,608,I(0xf56f8c),0);
    B(0xe38dca)=1;
}
