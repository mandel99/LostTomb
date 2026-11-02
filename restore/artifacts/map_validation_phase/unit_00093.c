#include "map_validation.h"
/* Original returns AL only; its two cached callers consume AL. */
uint8_t FUN_0040c8b0(int32_t product)
{
    uint32_t i,base=0x5d19ceu+(uint32_t)product*16u;int32_t requirement;
    MVE(0x40c8b0,product,0);
    for(i=0;i<3u;++i){
        requirement=(int32_t)(int16_t)MVR(base+i*2u,2);
        if(requirement&&!FUN_00442870((uint32_t)requirement))return 0;
    }
    return 1;
}
