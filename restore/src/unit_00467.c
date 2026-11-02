#include "camera_navigation.h"
#define CN_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void FUN_00430cd0(uint32_t tile)
{
    uint32_t x,y;
    FUN_004304c0(tile);
    x=CN_U(0xd3b4e4)-(uint32_t)((int32_t)CN_U(0x70e464)/2);
    y=CN_U(0xd3b4e0)-(uint32_t)((int32_t)CN_U(0x70e460)/2);
    CN_U(0xbca2bc)=x;
    CN_U(0xe38dcc)=1;
    CN_U(0xbca2c0)=y&0xfffeu;
    FUN_004de080();
}
