#include "map_refresh_postpass1.h"
void FUN_00477500(int32_t xmin,int32_t ymin,int32_t xmax,int32_t ymax)
{
    int32_t width,height,x,y;uint32_t index,next,flags,step,tile;
    if(xmin<0)xmin=0;if(ymin<0)ymin=0;
    width=(int32_t)P1_R(0xbca2d0u,4);if(xmax>=width)xmax=(int32_t)((uint32_t)width-1u);
    height=(int32_t)P1_R(0xbca2d4u,4);if(ymax>=height)ymax=(int32_t)((uint32_t)height-1u);
    step=(uint32_t)width-(uint32_t)xmax+(uint32_t)xmin-1u;
    index=(uint32_t)P1_R(0xbca2d8u,4)+(uint32_t)ymin*228u+(uint32_t)xmin;
    for(y=ymin;y<=ymax;++y){
        if(xmin<=xmax){
            tile=index;next=index+(uint32_t)xmax-(uint32_t)xmin+1u;
            for(x=xmin;x<=xmax;++x,++tile){
                flags=(uint32_t)P1_R(0xaf598cu+tile*4u,4);
                if((flags&0x800u)&&!(flags&0x1700u)&&!(flags&0x40u)&&(!(flags&8u)||(flags&0x10000000u))&&!(flags&0x20u))FUN_00479b70(x,y,1);
            }
            index=next;
        }
        index+=(uint32_t)P1_R(0xbca2dcu,4)+step;
    }
}
