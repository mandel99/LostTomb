#include "actor_coordinates.h"

void FUN_004f08c0(int32_t building,int32_t step,int32_t *x,int32_t *y)
{
    uint32_t record=(uint32_t)building*264u,tier,height,index,orientation,rotation,slot;
    if(step<1){ac_store(x,0);ac_store(y,15);return;}
    tier=((uint32_t)step-1u)/6u;
    if(AC_U(0x5e4ef4)==1)height=AC_U(0x5e68f8u+(tier+AC_B(0x799a2cu+record)*6u)*4u);
    else height=tier*90u;
    ac_store(x,AC_U(0x70e45c)+1u);
    ac_store(y,0u-(AC_U(0x70e44c)+height));
    if(AC_U(0x5e4ef4)==5){
        index=(uint32_t)AC_S(0x7999eau+record)+AC_B(0x799a2cu+record)*36u;
        orientation=AC_U(0x5e6578u+index*8u);
    }else{
        index=(uint32_t)AC_S(0x7999eau+record)+AC_B(0x799a2cu+record)*144u;
        orientation=AC_U(0x5e4ef8u+index*8u);
    }
    rotation=(uint32_t)((int32_t)AC_U(0xbca2e0)/2);
    if((int32_t)orientation<4){orientation+=rotation;if((int32_t)orientation>=4)orientation-=4u;}
    else if((int32_t)orientation<8){orientation+=rotation;if((int32_t)orientation>=8)orientation-=4u;}
    if((int32_t)orientation<9){
        if(AC_B(0x799a2fu+record)<AC_B(0x799a30u+record))orientation=8;
        slot=((uint32_t)step-1u)%6u+orientation*6u;
        if(AC_U(0x5e4ef4)==3){
            index=slot*16u;
            if(AC_U(0x799a40u+record)==0){
                ac_store(x,ac_out(x)+AC_U(0x5f57c8u+index));
                height=AC_U(0x5f57ccu+index);
            }else{
                ac_store(x,ac_out(x)+AC_U(0x5f57d0u+index));
                height=AC_U(0x5f57d4u+index);
            }
            ac_store(y,ac_out(y)+height);
            ac_store(x,ac_out(x)+5u);ac_store(y,ac_out(y)+8u);
            ac_store(x,ac_out(x)+1u);ac_store(y,ac_out(y)+19u);return;
        }
        index=slot*8u;
        ac_store(x,ac_out(x)+AC_U(0x5f5618u+index));
        height=AC_U(0x5f561cu+index);
        ac_store(y,ac_out(y)+height);
        if(AC_U(0x799a40u+record)!=0){
            ac_store(x,ac_out(x)+AC_U(0x5f5bf8u+orientation*8u));
            ac_store(y,ac_out(y)+AC_U(0x5f5bfcu+orientation*8u));
        }
    }
    ac_store(x,ac_out(x)+1u);ac_store(y,ac_out(y)+19u);
}
