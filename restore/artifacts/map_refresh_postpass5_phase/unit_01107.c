#include "map_refresh_postpass5.h"
void FUN_0047b590(int32_t xmin,int32_t ymin,int32_t xmax,int32_t ymax)
{
    int32_t width,height;uint32_t tile,step,column,row,flags,padding;
    if(xmin<0)xmin=0;if(ymin<0)ymin=0;
    width=(int32_t)P5_R(0xbca2d0u,4);if(xmax>=width)xmax=(int32_t)((uint32_t)width-1u);
    height=(int32_t)P5_R(0xbca2d4u,4);if(ymax>=height)ymax=(int32_t)((uint32_t)height-1u);
    P5_W(0xbc9760u,4,(uint32_t)ymin);step=(uint32_t)width-(uint32_t)xmax+(uint32_t)xmin-1u;
    tile=P5_R(0xbca2d8u,4)+(uint32_t)ymin*228u+(uint32_t)xmin;P5_W(0xbc9768u,4,tile);if(ymin>ymax)return;
    do{
        P5_W(0xbc9764u,4,(uint32_t)xmin);
        if(xmin<=xmax){do{
            flags=P5_R(0xaf598cu+tile*4u,4);
            if((flags&0x40u)&&!(flags&0xcu)){p5_gather(0x8000040u);FUN_0047e5c0();FUN_0047b670();tile=P5_R(0xbc9768u,4);}
            column=P5_R(0xbc9764u,4)+1u;++tile;P5_W(0xbc9764u,4,column);P5_W(0xbc9768u,4,tile);
        }while((int32_t)column<=xmax);}
        padding=P5_R(0xbca2dcu,4);row=P5_R(0xbc9760u,4)+1u;tile+=padding+step;P5_W(0xbc9760u,4,row);P5_W(0xbc9768u,4,tile);
    }while((int32_t)row<=ymax);
}
