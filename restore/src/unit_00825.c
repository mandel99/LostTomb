#include "actor_callbacks_small.h"
int32_t FUN_004578b0(int32_t building)
{
    uint32_t b,slot;int32_t count,index,value,result=0,resource;int16_t type;
    if(building<=0)return 0;b=as_building(building);type=AS_S(b+16u);if(type!=70&&type!=190)return 0;
    count=AS_I(0x00f53dd8u);slot=0x00f53ddcu;
    while(count>0){
        resource=AS_I(slot);
        if(resource){
            switch(resource){case 1:index=0;break;case 2:index=1;break;case 3:index=2;break;case 4:index=3;break;case 5:index=4;break;case 6:index=5;break;case 7:index=6;break;case 8:index=7;break;case 13:index=9;break;case 15:index=8;break;case 17:index=10;break;case 19:index=11;break;default:index=-1;break;}
            value=AS_S(b+0x56u+(uint32_t)index*2u);if(value>result)result=value;
        }
        slot+=4u;--count;
    }
    return result;
}
