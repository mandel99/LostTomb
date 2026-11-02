#include "map_validation.h"
uint32_t FUN_00443a80(uint32_t kind)
{
    uint32_t i,record=0x77f748u;
    MVE(0x443a80,kind,0);
    for(i=0;i<200u;++i,record+=98u)
        if(MVR(record+1u,1)&&MVR(record,1)==1u&&FUN_00443000(i,kind))return 1;
    return 0;
}
