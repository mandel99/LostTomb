#include "actor_callbacks_small.h"
int32_t FUN_004579d0(int32_t building)
{
    uint32_t b,offset;int32_t value,result=0;int16_t type;
    if(building<=0)return 0;b=as_building(building);type=AS_S(b+16u);if(type!=70&&type!=190)return 0;
    for(offset=0x66;offset<=0x6c;offset+=2){value=AS_S(b+offset);if(value>result)result=value;}
    return result;
}
