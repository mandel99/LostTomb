#include "actor_callbacks_chain.h"
int32_t FUN_0049b3c0(int32_t actor)
{
    int32_t predecessor=AC_S(ac_actor((uint32_t)actor)+0x60u);unsigned count;
    for(count=0;count<8;++count){
        uint32_t a=ac_actor((uint32_t)predecessor);
        if(AC_B(a+2u)==39)return predecessor;
        predecessor=AC_S(a+0x60u);
    }
    return 0;
}
