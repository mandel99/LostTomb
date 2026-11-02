#include "order_input.h"
uint32_t FUN_0040b630(void)
{
    uint32_t y,base,tile;int32_t group;
    if(OI_B(0xe38ddau)==1&&OI_U(0x6059e8u))return 0;
    if(!OI_U(0xbca2c4u))return 0;
    y=OI_U(0xbca2ccu);base=OI_U(0xbca2d8u);tile=OI_U(0xbca2c8u)+y*228u+base;
    group=FUN_004b5520(tile);
    if(group<=0||OI_S(0xbcba4au+(uint32_t)group*144u)>=0)return 0;
    if(FUN_0040b5e0(group))return 0;
    OI_W(0xbcb9c0u,group);OI_WB(0xe38ddau,0x1e);OI_WB(0xe38dcau,1);
    return 1;
}
