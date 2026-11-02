#include "order_actions_finish.h"
uint32_t FUN_004b66e0(int32_t x,int32_t y,int32_t group_id,uint32_t task)
{
    static const int32_t delta[9][2]={{-1,-1},{-1,0},{-1,1},{0,-1},{0,0},{0,1},{1,-1},{1,0},{1,1}};
    uint32_t group=oa_group(group_id),leader,actor,index,cell;uint8_t blocked=OA_B(0xe38e60u);
    int32_t id=OA_S(group+64u),gx,gy,cx=0,cy=0,ax,ay;int16_t old_x;
    if(blocked)return 0;
    gy=OA_S(group+48u);gx=OA_S(group+46u);(void)FUN_00517a40(gx,gy,-2,-2);
    leader=oa_actor(id);
    for(index=0;index<9;++index){
        cy=(int32_t)((uint32_t)y+(uint32_t)delta[index][1]);cx=(int32_t)((uint32_t)x+(uint32_t)delta[index][0]);
        ay=OA_S(leader+22u);ax=OA_S(leader+20u);
        if(FUN_00518400(ax,ay,cx,cy))break;
    }
    if(index==9){restore_actor_order_message(0xd3,0,0);return 0;}
    cell=(uint32_t)cx+(uint32_t)cy*228u+OA_U(0xbca2d8u);
    if((int32_t)OA_U(0xeed760u+cell*4u)<=0)return 0;
    old_x=OA_S(group+46u);
    if(cx==old_x&&cy==OA_S(group+48u))return 0;
    if(OA_S(group+100u))return 0;
    OA_WS(group+50u,old_x);OA_WS(group+52u,OA_S(group+48u));OA_WB(group+3u,0);OA_W(group+140u,task);
    if(OA_S(group+44u)<=20)restore_actor_order_message(0x33,0,0);
    if(OA_S(group+4u)==13){
        id=OA_S(group+8u);
        if(OA_B(oa_actor(id)+56u)==19&&OA_U(0xd3b198u)!=1){
            for(index=0;index<16;++index){
                id=OA_S(group+8u+index*2u);if(!id)break;actor=oa_actor(id);
                if(!oa_eligible(group,actor,id,0))continue;
                if(OA_B(group+102u)==1)FUN_004b7b50(group_id,10);
                OA_WB(actor,0);OA_W(actor+308u,0);OA_W(actor+316u,(int32_t)OA_S(actor+20u));OA_W(actor+320u,(int32_t)OA_S(actor+22u));
                OA_WB(actor+56u,21);OA_WS(actor+130u,0);OA_WB(oa_actor((int32_t)OA_U(0xd056f8u))+1u,0);
            }
        }
    }
    for(index=0;index<16;++index){
        id=OA_S(group+8u+index*2u);if(!id)break;actor=oa_actor(id);
        if(!oa_eligible(group,actor,id,1))continue;
        if(OA_B(group+102u)==1)FUN_004b7b50(group_id,10);
        OA_WB(actor,0);OA_WB(actor+56u,21);OA_WS(actor+130u,0);FUN_004b3250(id);
    }
    return 1;
}
