#include "menu_group_selection.h"
#include <stddef.h>

/* Native 00406720 and 004b7b50. Group indices are DWORDs; actor indices
   are signed WORDs. All address arithmetic intentionally wraps at 32 bits. */
#pragma pack(push,1)
typedef struct SelectionGroup {
    uint8_t opaque0[4]; int16_t kind; uint8_t opaque6[2];
    int16_t actors[16]; uint8_t opaque40[2];
    int16_t order,health; uint8_t opaque46[26];
    int16_t leader; uint8_t opaque74[24]; int16_t previous_order;
    uint8_t opaque100[2],order_mode,opaque103[3],subtype,opaque107[8],disabled;
    uint8_t opaque116[3],alternate,opaque120[4];
    int16_t resume_order,formation; uint8_t opaque128[2]; int16_t availability;
    uint8_t opaque132[8]; uint32_t task;
} SelectionGroup;
typedef struct SelectionActor {
    uint8_t status,marker,opaque2[18]; int16_t x,y;
    uint8_t opaque24[32],mode,opaque57[251]; uint32_t route;
    uint8_t opaque312[4]; int32_t target_x,target_y; uint8_t opaque324[64];
} SelectionActor;
#pragma pack(pop)
typedef char group_size_check[sizeof(SelectionGroup)==144?1:-1];
typedef char actor_size_check[sizeof(SelectionActor)==388?1:-1];
typedef char group_availability_check[offsetof(SelectionGroup,availability)==130?1:-1];
typedef char actor_route_check[offsetof(SelectionActor,route)==308?1:-1];
#define U8(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U32(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
static volatile SelectionGroup *selection_group(uint32_t index)
{ return (volatile SelectionGroup *)(uintptr_t)(0x00bcb9c8u+index*144u); }
static volatile SelectionActor *selection_actor(uint32_t index)
{ return (volatile SelectionActor *)(uintptr_t)(0x00bcd5f8u+index*388u); }
extern uint32_t FUN_0047d350(void); /* Authenticated native leaf: 33 c0 c3. */

void FUN_004b7b50(int32_t group,int16_t delta)
{
    volatile SelectionGroup *g; int32_t cap; uint16_t health;
    if(!group)return;
    g=selection_group((uint32_t)group);
    if(g->kind==13)cap=g->leader!=0?100:80;
    else if(g->kind==57)cap=100;
    else if(g->kind==12||g->kind==11)cap=g->leader!=0?80:60;
    else cap=g->subtype==13?80:70;
    health=(uint16_t)((uint16_t)g->health+(uint16_t)delta);
    g->health=(int16_t)health;
    if((int16_t)health>cap)g->health=(int16_t)cap;
    if(g->health<0)g->health=0;
}

void FUN_00406720(void)
{
    uint32_t group=U32(0x00eadcccu),mode,selected_actor,offset;
    volatile SelectionGroup *g=selection_group(group);
    volatile SelectionActor *a; int16_t id,order; unsigned j;
    if(g->availability>=0)return;
    if(U8(0x00e38e60u))return;
    mode=U32(0x00d3b198u);
    if(mode==3){order=g->order;if(order!=6)g->previous_order=order;}
    if(g->kind==13&&selection_actor((uint32_t)(int32_t)g->actors[0])->mode==19&&mode!=1){
        selected_actor=U32(0x00d056f8u);
        for(j=0;j<16;++j){
            id=g->actors[j];if(!id)break;
            offset=(uint32_t)(int32_t)id;
            a=selection_actor(offset);
            if(a->mode==4||g->disabled||g->order_mode>1)continue;
            if(g->order_mode==1){FUN_004b7b50((int32_t)group,10);selected_actor=U32(0x00d056f8u);}
            a->status=0;a->route=0;
            a->target_x=a->x;a->target_y=a->y;a->mode=12;
            selection_actor(selected_actor)->marker=0;
            mode=U32(0x00d3b198u);
        }
    }
    /* Original table 004069c4: 0040686a,004068a2,0040690f,00406923.
       The callee ignores all three cdecl arguments and returns zero. */
    switch(mode){
    case 0:g->order=g->alternate?2:1;(void)FUN_0047d350();break;
    case 1:
        if(g->kind==13){
            if(g->formation==0){order=g->order;if(order)g->resume_order=order;g->order=0;}
        }else g->order=g->alternate?4:3;
        (void)FUN_0047d350();break;
    case 2:g->order=15;(void)FUN_0047d350();break;
    case 3:g->order=6;(void)FUN_0047d350();break;
    default:break;
    }
    U32(0x00bcb9c0u)=group;
    order=g->order;
    if(order==6){g->task=UINT32_MAX;U8(0x00e38ddau)=1;U8(0x00e38dcau)=1;return;}
    if(!order){g->task=UINT32_MAX;U8(0x00e38ddau)=0x49;U8(0x00e38dcau)=1;return;}
    if(g->formation){U8(0x00e38ddau)=9;if(U32(0x00d3b198u)!=1)U8(0x00e38ddau)=0x1e;}
    else U8(0x00e38ddau)=0x1e;
    U8(0x00e38dcau)=1;g->task=UINT32_MAX;
}
