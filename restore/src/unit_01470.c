#include "order_actions_finish.h"
void FUN_004b6c20(int32_t group_id,int32_t command)
{
    uint32_t group=oa_group(group_id),actor,cell,index;int32_t leader=OA_S(group+64u),id,x,y,ax,ay,out_x,out_y;
    if(command&&(id=(int32_t)OA_U(group+132u))!=0){
        actor=oa_actor(id);OA_WB(actor+56u,command);
        if(command==-1){x=OA_S(group+46u);y=OA_S(group+48u);}else{x=OA_S(group+54u);y=OA_S(group+56u);}
        ay=OA_S(actor+22u);ax=OA_S(actor+20u);(void)FUN_0040c180(ax,ay,x,y,&out_x,&out_y);
        OA_WS(actor+36u,out_x);OA_WS(actor+38u,out_y);OA_WS(actor+48u,x);OA_WS(actor+50u,y);FUN_004b3250(id);return;
    }
    y=OA_S(group+48u);x=OA_S(group+46u);(void)FUN_00517a40(x,y,-2,-2);
    y=OA_S(group+56u);cell=OA_U(0xbca2d8u)+(uint32_t)y*228u;x=OA_S(group+54u);cell+=(uint32_t)x;
    actor=oa_actor(leader);ay=OA_S(actor+22u);ax=OA_S(actor+20u);
    if(!FUN_00518400(ax,ay,x,y)){restore_actor_order_message(0xd3,0,0);return;}
    if((int32_t)OA_U(0xeed760u+cell*4u)<=0||OA_S(group+100u))return;
    OA_WB(group+3u,1);
    for(index=0;index<16;++index){
        id=OA_S(group+8u+index*2u);if(!id)break;actor=oa_actor(id);
        if(!oa_eligible(group,actor,id,1))continue;
        if(OA_B(group+102u)==1)FUN_004b7b50(group_id,10);
        OA_WB(actor+56u,9);FUN_004b3250(id);
    }
}
