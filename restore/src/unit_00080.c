#include "order_input.h"
uint32_t FUN_0040b5e0(int32_t group)
{
    uint32_t address=0xbcb9d0u+(uint32_t)group*144u,index;
    for(index=0;index<16;++index,address+=2u){
        int32_t actor=OI_S(address);
        if(!actor)return 0;
        if(OI_B(0xbcd630u+(uint32_t)actor*388u)==0x12)return 1;
    }
    return 0;
}
