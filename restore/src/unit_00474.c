#include "overlay_footprints.h"

/* Whole4321A0: water footprint tiles. Building ids are signed WORDs and
   record addresses use the264-byte stride exactly once. */
void FUN_004321a0(int32_t x,int32_t y,uint32_t flags,uint8_t transparent)
{
    uint32_t tile=GR_U(0x70e488u),record,count,table,image;
    int32_t building=(int16_t)GR_W(0xb4e6fcu+tile*2u);
    uint8_t status;
    record=(uint32_t)building*264u;status=GR_B(0x7999ddu+record);
    if(status&&GR_B(0x799a4fu+record)){
        FUN_00432d30(building,x,y,0);return;
    }
    table=overlay_pairs(GR_B(0xb285ccu+tile)&15u,&count);
    image=GR_U(0xf56d14u);if(status)image=GR_U(0xf56d1cu);
    if((flags&0x2000u)||(status&&GR_B(0x799a4fu+record)))image+=2u;
    else if(flags==0x80u)++image;
    overlay_draw_pairs(table,count,image,(uint32_t)x,(uint32_t)y,transparent!=0,0);
}
