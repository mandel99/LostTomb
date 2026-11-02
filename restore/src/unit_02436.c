#include "display_confirmation.h"
extern void FUN_004cf1c0(void);
extern void FUN_00536820(int x,int y,int width,int height);
extern void FUN_004d6160(int group,int entry,int x,int y,int width,uint32_t font,int flags,int screen_x,int screen_y);
void FUN_00534540(void)
{
    int x=(int)(DC_U(0xe38da8)+80u),y=(int)(DC_U(0xe38da4)+64u);
    FUN_004cf1c0();
    FUN_00536820(x,y,30,10);
    FUN_004d6160(0x92,0,(int)((uint32_t)x+32u),(int)((uint32_t)y+30u),416,
        DC_U(0xf56f8c),0,DC_I(0xe38da8),DC_I(0xe38da4));
}
