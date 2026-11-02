#include "order_input.h"
static uint32_t selected(int32_t actor)
{
    OI_W(0xd05704u,actor);OI_W(0xd0570cu,1);OI_WB(0xe38dcau,1);OI_WB(0xe38ddau,0x20);
    return 1;
}
uint32_t FUN_0040b6d0(void)
{
    uint32_t y,tile,address;int32_t actor;
    if(OI_B(0xe38ddau)==1&&OI_U(0x6059e8u))return 0;
    if(!OI_U(0xbca2c4u))return 0;
    y=OI_U(0xbca2ccu);tile=OI_U(0xbca2c8u)+y*228u;tile+=OI_U(0xbca2d8u);
    actor=FUN_004b55b0(tile);
    if(actor>0&&OI_S(0xbcd72au+(uint32_t)actor*388u)>=0)return selected(actor);
    for(address=0x5f80fcu;address<0x5f811cu;address+=4u){
        actor=FUN_004b55b0(OI_U(address)+tile);
        /* The native neighbour predicate has the opposite sign to the centre. */
        if(actor>0&&OI_S(0xbcd72au+(uint32_t)actor*388u)<0)return selected(actor);
    }
    return 0;
}
