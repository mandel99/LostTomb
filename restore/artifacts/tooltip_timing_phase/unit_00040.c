#include "tooltip_timing.h"
int32_t FUN_00405170(uint32_t group)
{
    uint32_t count,base=0x5cf75cu+group*120u;
    TTE(0x405170,group);
    for(count=0;count<30u;++count)if((int32_t)TTR(base+count*4u)<=0)break;
    return (int32_t)count;
}
