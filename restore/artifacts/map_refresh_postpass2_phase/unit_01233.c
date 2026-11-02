#include "map_refresh_postpass2.h"
int32_t FUN_00484be0(void *records,int32_t count,int32_t animation,uint8_t reverse)
{
    int32_t direction=p2_direction(),ordinal;uint32_t row=(uint32_t)(uintptr_t)records,i,control,neighbor,frames;
    if(reverse)row+=(uint32_t)count*16u-16u;
    if(count<=0)return 0;
    for(ordinal=0;ordinal<count;++ordinal){
        for(i=0;i<8;++i){control=P2_R(row+i,1);if(control==2)continue;neighbor=P2_R(0xbc97c0u+i*4u,4);if((control!=0)!=(neighbor!=0))break;}
        if(i==8){
            p2_publish(row,direction);
            if(reverse?animation==0:animation<0)p2_cycle(row);
            else{frames=P2_R(row+13u,1);P2_W(0xbc9740u,4,(uint32_t)(animation%(int32_t)frames));}
            return ordinal+1;
        }
        row+=reverse?(uint32_t)-16:16u;
    }
    return 0;
}
