#include "display_configuration.h"
#define I32(address) (*(int32_t *)(address##u))
#define U32(address) (*(uint32_t *)(address##u))
void restore_display_configure(uint32_t mode)
{
    int32_t width,height;
    FUN_004cfd60((int)mode);
    U32(0x00e38e68)=mode;
    width=I32(0x00e38db0); height=I32(0x00e38dac);
    I32(0x00e38d9c)=width;
    I32(0x00e38da8)=(int32_t)((uint32_t)width-640u)/2;
    I32(0x00e38da4)=(int32_t)((uint32_t)height-480u)/2;
    U32(0x00e38d94)=(uint32_t)width*2u;
    U32(0x00e38d8c)=U32(0x00e38d94)*(uint32_t)height;
    I32(0x00e38d98)=height; I32(0x00e38d90)=height;
    I32(0x00d3b55c)=0; I32(0x00d3b558)=width;
    I32(0x00d3b554)=0; I32(0x00d3b550)=height;
}
