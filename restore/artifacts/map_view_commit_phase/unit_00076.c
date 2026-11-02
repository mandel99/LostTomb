#include "map_view_commit.h"
void FUN_0040a090(void)
{
    uint32_t index,a,b;
    MCE(0x40a090);
    if(MCR(0x605a3cu,4)==1u){
        index=MCR(0x784f10u,4);b=MCR(0x775910u,2);a=MCR(0x77590cu,2);
        MCW(0x7848dcu,2,index);
        MCW(0x7843d8u+index*32u,2,b);MCW(0x7843dau+index*32u,2,a);
        FUN_00442800();
    }
    MCW(0xe38ddau,1,1);MCW(0xe38dcau,1,1);
}
