#include "order_actions_finish.h"
void FUN_0040c570(void)
{
    uint32_t tile,group,group_address,ty;int32_t building;
    if(OA_B(0xe38e60u))return;
    if(!OA_U(0xbca2c4u))goto finish;
    ty=OA_U(0xbca2ccu);tile=OA_U(0xbca2c8u)+ty*228u;tile+=OA_U(0xbca2d8u);
    group=OA_U(0xbcb9c0u);group_address=oa_group((int32_t)group);
    if(OA_S(group_address+130u)>=0||OA_S(group_address+100u))return;
    building=FUN_004b55f0(tile);group=OA_U(0xbcb9c0u);
    if(building&&(uint32_t)building==group){FUN_004b6c20((int32_t)group,14);goto finish;}
    tile=OA_U(0xbca2c8u);ty=OA_U(0xbca2ccu);(void)FUN_004b6a40((int32_t)tile,(int32_t)ty,(int32_t)group);
    (void)FUN_0047d350();
finish:OA_WB(0xe38ddau,1);OA_WB(0xe38dcau,1);
}
