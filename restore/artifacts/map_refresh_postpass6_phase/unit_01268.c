#include "map_refresh_postpass6.h"
void FUN_00487b70(int32_t x,int32_t y)
{
    uint32_t xmin=(uint32_t)x-1u,xmax,width,ymin=(uint32_t)y-1u,ymax,height,padding,step,tile,row,column,flags,remove;
    P6_RECURSE(0,x,y);width=P6_R(0xbca2d0u,4);
    if((int32_t)xmin<0)xmin=0;xmax=(uint32_t)x+1u;if((int32_t)width<(int32_t)xmax)xmax=width;
    if((int32_t)ymin<0)ymin=0;height=P6_R(0xbca2d4u,4);ymax=(uint32_t)y+1u;if((int32_t)height<(int32_t)ymax)ymax=height;
    padding=P6_R(0xbca2dcu,4);step=padding-xmax+width+xmin-1u;tile=P6_R(0xbca2d8u,4)+ymin*228u+xmin;row=ymin;
    if((int32_t)row<=(int32_t)ymax){do{
        column=xmin;
        if((int32_t)xmin<=(int32_t)xmax){do{
            flags=P6_R(p6_address(tile),4);
            if(flags&0x4000000u){
                remove=!FUN_00487790((int32_t)column,(int32_t)row,0x4000000u);
                if(!remove)remove=!FUN_00487730((int32_t)column,(int32_t)row,0x4000000u);
                if(!remove){P6_W(0xbc9768u,4,tile);p6_gather(0x4000000u);remove=p6_has_two_run();}
                if(remove){flags=P6_R(p6_address(tile),4);P6_W(p6_address(tile),4,flags&0xfbffffffu);FUN_00487b70((int32_t)column,(int32_t)row);}
            }
            ++column;++tile;
        }while((int32_t)column<=(int32_t)xmax);}
        ++row;tile+=step;
    }while((int32_t)row<=(int32_t)ymax);}
    P6_RECURSE(1,x,y);
}
