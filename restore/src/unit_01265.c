#include "map_refresh_postpass6.h"
uint32_t FUN_00487730(int32_t x,int32_t y,uint32_t mask)
{
    uint32_t count=0,column=(uint32_t)x-1u,end=(uint32_t)x+2u,base;
    if((int32_t)column>=(int32_t)end)return 0;
    base=P6_R(0xbca2d8u,4);
    do{
        if((int32_t)column>=0&&(int32_t)column<(int32_t)P6_R(0xbca2d0u,4))if(P6_R(p6_address(base+(uint32_t)y*228u+column),4)&mask)++count;
        ++column;
    }while((int32_t)column<(int32_t)end);
    return count>1u;
}
