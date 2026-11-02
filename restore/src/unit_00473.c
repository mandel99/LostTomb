#include "overlay_footprints.h"

/* Whole431CE0: city water-overlay ground pass. Preserve native filtering,
   callback reloads and the different unsupported-footprint branches. */
void FUN_00431ce0(int32_t x,int32_t y)
{
    uint32_t tile=GR_U(0x70e488u),flags=GR_U(0xaf598cu+tile*4u);
    uint32_t image,px,py,f,record,count,table;
    int32_t building;
    uint8_t status;
    if(flags&0x26341777u){
        if(flags&8u){
            GR_U(0xd3b5c0u)=GR_U(0xf56bdcu);
            FUN_00432d30((int16_t)GR_W(0xb4e6fcu+tile*2u),x,y,0);return;
        }
        image=GR_U(0xb7482cu+tile*4u);f=GR_B(0xb285ccu+tile)&15u;
        GR_U(0xd3b5c0u)=image;
        if(ground_footprint(f,(uint32_t)x,(uint32_t)y,&px,&py))
            (void)FUN_0053e590(GR_S(image),GR_S(px),GR_S(py));
        return;
    }
    if(flags&0x40000000u){
        building=(int16_t)GR_W(0xb4e6fcu+tile*2u);record=(uint32_t)building*264u;
        status=GR_B(0x7999ddu+record);
        if(status&&GR_B(0x799a4fu+record)){FUN_00432d30(building,x,y,0);return;}
        table=overlay_pairs(GR_B(0xb285ccu+tile)&15u,&count);
        image=GR_U(0xf56d14u);if(status)image=GR_U(0xf56d1cu);
        if((flags&0x2000u)||(status&&GR_B(0x799a4fu+record)))image+=2u;
        else if(flags==0x80u)++image;
        overlay_draw_pairs(table,count,image,(uint32_t)x,(uint32_t)y,0,0);return;
    }
    if(flags&0x4000u){
        image=(GR_B(0xacf85cu+tile)&7u)+GR_U(0xf56b8cu);
    }else if(!(flags&8u)){
        image=GR_U(0xf56d14u);
        if(flags&0x2000u)image+=2u;else if((flags&0x2080u)==0x80u)++image;
    }else{
        building=(int16_t)GR_W(0xb4e6fcu+tile*2u);
        if((uint8_t)FUN_00431690(building)==0){FUN_004321a0(x,y,flags&0x2080u,0);return;}
        tile=GR_U(0x70e488u);f=GR_B(0xb285ccu+tile)&15u;
        image=GR_U(0xb7482cu+tile*4u);GR_U(0xd3b5c0u)=image;
        px=(uint32_t)x;py=(uint32_t)y;
        (void)ground_footprint(f,(uint32_t)x,(uint32_t)y,&px,&py);
        (void)FUN_0053e590(GR_S(image),GR_S(px),GR_S(py));return;
    }
    GR_U(0xd3b5c0u)=image;(void)FUN_0053e590(GR_S(image),x,y);
}
