#include "ground_render.h"

void FUN_00430e60(uint32_t x,uint32_t y,uint32_t mode,uint32_t actor_parameter)
{
    uint32_t left,right,top,bottom,width,height;
    GR_U(0x70e478)=x;left=GR_U(0x70e43c);
    GR_U(0x70e474)=y;right=GR_U(0x70e438);
    GR_U(0x70e484)=mode;top=GR_U(0x70e434);
    GR_U(0xd3b55c)=left;bottom=GR_U(0x70e430);
    GR_U(0xd3b558)=right;GR_U(0xd3b554)=top;GR_U(0xd3b550)=bottom;
    FUN_00430ef0();FUN_00436b60(actor_parameter);FUN_00439740(0);
    width=GR_U(0xe38db0);height=GR_U(0xe38dac);
    GR_U(0xd3b55c)=0;GR_U(0xd3b558)=width;GR_U(0xd3b554)=0;GR_U(0xd3b550)=height;
}
