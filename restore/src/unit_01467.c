#include "actor_order_action.h"
#include "actor_order_path.h"
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_004b6400(int32_t x,int32_t y,int32_t group_id,uint32_t flags)
{
    uint32_t offset=(uint32_t)group_id*144u,index,actor,selected;int32_t id;
    RestoreActionGroup *group=(RestoreActionGroup *)(uintptr_t)(0xbcb9c8u+offset);int16_t leader;
    if(B(0xe38e60))return 0;
    leader=group->leader;
    (void)FUN_00517a40(group->x,group->y,-2,-2);
    if(group->direct_origin){if(!FUN_00518400(group->x,group->y,x,y)){restore_actor_order_message(0xd3,0u,0u);return 0;}}
    else if(!FUN_00518400(W(0xbcd60cu+(uint32_t)(int32_t)leader*388u),W(0xbcd60eu+(uint32_t)(int32_t)leader*388u),x,y)){restore_actor_order_message(0xd3,0u,0u);return 0;}
    if(I(0xeed760u+(U(0xbca2d8)+(uint32_t)y*228u+(uint32_t)x)*4u)<=0)return 0;
    if(x==group->x&&y==group->y)return 0;
    if(group->locked)return 0;
    group->target_x=(int16_t)x;group->target_y=(int16_t)y;group->marker=0;
    if((uint8_t)flags)return 1;
    if(group->health<=20)restore_actor_order_message(0x33,0u,0u);
    if(group->kind==13&&B(0xbcd630u+(uint32_t)(int32_t)group->actors[0]*388u)==19&&U(0xd3b198)!=1){
        for(index=0;index<16;++index){
            id=group->actors[index];if(!id)break;actor=(uint32_t)id*388u;
            if(B(0xbcd630u+actor)==4||group->disabled||group->order_mode>1||(uint8_t)FUN_004b63d0(id))continue;
            if(group->order_mode==1)FUN_004b7b50(group_id,10);
            selected=U(0xd056f8);B(0xbcd5f8u+actor)=0;U(0xbcd72cu+actor)=0;group->action_tag=15;
            I(0xbcd734u+actor)=W(0xbcd60cu+actor);I(0xbcd738u+actor)=W(0xbcd60eu+actor);B(0xbcd630u+actor)=20;
            B(0xbcd5f9u+selected*388u)=0;
        }
    }
    for(index=0;index<16;++index){
        id=group->actors[index];if(!id)return 1;actor=(uint32_t)id*388u;
        if(B(0xbcd630u+actor)==4||B(0xbcd630u+actor)==5||group->disabled||group->order_mode>1||(uint8_t)FUN_004b63d0(id))continue;
        if(group->order_mode==1)FUN_004b7b50(group_id,10);
        B(0xbcd5f8u+actor)=0;B(0xbcd630u+actor)=11;FUN_004b3250(id);
    }
    return 1;
}
