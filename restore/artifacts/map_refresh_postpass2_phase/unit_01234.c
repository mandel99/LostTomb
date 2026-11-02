#include "map_refresh_postpass2.h"
int32_t FUN_00484d70(void *records,int32_t count)
{
    static const uint8_t groups[16]={0,1,3,2,5,1,4,2,6,7,1,1,5,1,1,1};
    uint32_t mask=0,side,value,group,offset,row,i,control,neighbor;int32_t direction,ordinal;
    for(side=0;side<4;++side){
        value=P2_R(0xbc97c4u+side*8u,1);if(value&2u)mask|=(1u<<side)|(1u<<((side+1u)&3u));
        value=P2_R(0xbc97c0u+side*8u,1);if(value&2u)mask|=1u<<side;
    }
    group=groups[mask];offset=group*(uint32_t)count;row=(uint32_t)(uintptr_t)records+offset*16u;direction=p2_direction();
    if(count<=0)return 0;
    for(ordinal=0;ordinal<count;++ordinal){
        for(i=0;i<8;++i){control=P2_R(row+i,1);if(control==2)continue;neighbor=P2_R(0xbc97c0u+i*4u,1)&1u;if((control!=0)!=(neighbor!=0))break;}
        if(i==8){p2_publish(row,direction);p2_cycle(row);return (int32_t)(offset+(uint32_t)ordinal+1u);}
        row+=16u;
    }
    return 0;
}
