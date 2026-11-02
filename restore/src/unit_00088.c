#include "order_actions_finish.h"
void FUN_0040c440(void)
{
    uint32_t tile,group_address,group,ty,base;int32_t building,actor,tag;uint8_t kind;
    if(OA_B(0xe38e60u))return;
    if(!OA_U(0xbca2c4u))goto finish;
    ty=OA_U(0xbca2ccu);base=OA_U(0xbca2d8u);tile=OA_U(0xbca2c8u)+ty*228u+base;
    group=OA_U(0xbcb9c0u);group_address=oa_group((int32_t)group);
    if(OA_S(group_address+130u)>=0||OA_S(group_address+100u))return;
    building=FUN_004b55f0(tile);actor=FUN_004b5640(tile);group=OA_U(0xbcb9c0u);
    if(building&&(uint32_t)building==group){FUN_004b6c20((int32_t)group,14);goto finish;}
    tag=-1;kind=OA_B(oa_actor(actor)+10u);
    if(kind!=0x5c&&kind!=0x65&&kind!=0x5d&&kind!=0x64)tag=OA_S(oa_actor(actor)+96u);
    ty=OA_U(0xbca2ccu);tile=OA_U(0xbca2c8u);
    if(actor)(void)FUN_004b66e0((int32_t)tile,(int32_t)ty,(int32_t)group,(uint32_t)tag);
    else(void)FUN_004b6400((int32_t)tile,(int32_t)ty,(int32_t)group,0);
    (void)FUN_0047d350();
finish:OA_WB(0xe38ddau,1);OA_WB(0xe38dcau,1);
}
