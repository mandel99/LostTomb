#include "map_refresh_postpass5.h"
uint32_t FUN_0047b920(void)
{
    uint32_t tile=P5_R(0xbc9768u,4),flags=P5_R(0xaf598cu+tile*4u,4);int32_t height;
    if(flags&0x10000u)return 0;
    height=(int8_t)P5_R(0xac2d4cu+tile,1);
    if(height>4)return 1;if(height<=0)return 0;return (flags>>13)&1u;
}
