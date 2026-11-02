#include "order_input.h"
static uint32_t selected(int32_t actor)
{
    OI_W(0xd05700u,actor);OI_W(0xd05708u,1);OI_WB(0xe38dcau,1);OI_WB(0xe38ddau,0x1f);
    return 1;
}
uint32_t FUN_0040b790(void)
{
    uint32_t y,tile,address;int32_t actor;
    if(OI_B(0xe38ddau)==1&&OI_U(0x6059e8u))return 0;
    if(!OI_U(0xbca2c4u))return 0;
    y=OI_U(0xbca2ccu);tile=OI_U(0xbca2c8u)+y*228u;tile+=OI_U(0xbca2d8u);
    actor=FUN_004b5570(tile);
    if(actor>0)return selected(actor);
    for(address=0x5f80fcu;address<0x5f811cu;address+=4u){
        actor=FUN_004b5570(OI_U(address)+tile);
        if(actor>0)return selected(actor);
    }
    return 0;
}
