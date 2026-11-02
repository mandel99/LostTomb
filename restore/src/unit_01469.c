#include "order_actions_finish.h"
uint32_t FUN_004b6a40(int32_t x,int32_t y,int32_t group_id)
{
    uint32_t group=oa_group(group_id),actor,index,cell;uint8_t blocked=OA_B(0xe38e60u);
    int32_t id=OA_S(group+64u),gx,gy,ax,ay;
    if(blocked)return 0;
    gy=OA_S(group+48u);gx=OA_S(group+46u);(void)FUN_00517a40(gx,gy,-2,-2);
    actor=oa_actor(id);ay=OA_S(actor+22u);ax=OA_S(actor+20u);
    if(!FUN_00518400(ax,ay,x,y)){restore_actor_order_message(0xd3,0,0);return 0;}
    cell=OA_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    if((int32_t)OA_U(0xeed760u+cell*4u)<=0)return 0;
    if(x==OA_S(group+46u)&&y==OA_S(group+48u))return 0;
    if(OA_S(group+100u))return 0;
    OA_WS(group+50u,x);OA_WS(group+52u,y);OA_WS(group+126u,1);OA_WS(group+128u,0);OA_WB(group+3u,0);
    if(OA_S(group+44u)<=20)restore_actor_order_message(0x33,0,0);
    OA_WS(group+42u,0);
    for(index=0;index<16;++index){
        id=OA_S(group+8u+index*2u);if(!id)break;actor=oa_actor(id);
        if(!oa_eligible(group,actor,id,1))continue;
        if(OA_B(group+102u)==1)FUN_004b7b50(group_id,10);
        OA_WB(actor,0);OA_W(actor+308u,0);OA_W(actor+316u,(int32_t)OA_S(actor+20u));OA_W(actor+320u,(int32_t)OA_S(actor+22u));
        OA_WB(actor+56u,19);FUN_004b3250(id);
    }
    return 1;
}
