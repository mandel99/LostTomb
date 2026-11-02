#include "map_refresh_postpass6.h"
uint32_t FUN_00487790(int32_t x,int32_t y,uint32_t mask)
{
    uint32_t count=0,row=(uint32_t)y-1u,end=(uint32_t)y+2u,base,height,tile;
    if((int32_t)row>=(int32_t)end)return 0;
    base=P6_R(0xbca2d8u,4);height=P6_R(0xbca2d4u,4);tile=base+row*228u+(uint32_t)x;
    do{
        if((int32_t)row>=0&&(int32_t)row<(int32_t)height)if(P6_R(p6_address(tile),4)&mask)++count;
        ++row;tile+=228u;
    }while((int32_t)row<(int32_t)end);
    return count>1u;
}
