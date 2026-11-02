#include "order_input.h"
int32_t FUN_004b55b0(uint32_t tile)
{
    int32_t actor=OI_S(0xb350dcu+tile*2u);
    while(actor){
        uint32_t address=0xbcd5f8u+(uint32_t)actor*388u;
        if(OI_B(address+10u)==0x4e)return actor;
        actor=OI_S(address+8u);
    }
    return 0;
}
