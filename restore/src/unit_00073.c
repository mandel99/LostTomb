#include "tooltip_timing.h"
uint32_t FUN_00409e40(void)
{
    uint32_t selector,now,last,value;
    TTE(0x409e40,0);selector=TTR(0x7056c8u);
    if(selector==0u&&TTR(0x7056c4u)==0u){
        now=TTR(0xe92e0cu);last=TTR(0x605790u);
        if((int32_t)(now-last)>500){TTW(0x7056dcu,0);TTW(0x7056d8u,0);}
        return 0;
    }
    now=TTR(0xe92e0cu);TTW(0x605790u,now);
    if(selector){
        value=TTR(0x576180u+selector*4u);
        TTW(0x7056d0u,selector-1u);TTW(0x7056dcu,value);
        TTW(0x7056d8u,(uint32_t)FUN_00405170(value));
    }
    return 0;
}
