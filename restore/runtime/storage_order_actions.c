#include "storage_order_actions.h"
#include "menu_list_scroll.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole406300/92 instructions, all four real jump-table targets. The
   filtered native buffer starts one DWORD above the saved EBP slot; hence
   its read[entrySP-148+ordinal*4] is local[ordinal-1]. Native5C68C8 has30
   usable rows with primary1..30; following zero rectangles do not dispatch.
   Parent4F9819..4F9847 computes the count using this exact5013A0 predicate.
   Native5596D0 clears all36 slots and publishes at most35 resource IDs, so
   the parent count and all36-slot filter agree on every enabled row.
   These actual UI routes define each local read. No ordinal clamp, invented
   saved EBP value or zero initialization of inactive local slots is added. */
static void storage_toggle(void)
{
    uint32_t building=U(0xeadd6cu),offset=building*264u,owner,ordinal,resource;
    uint32_t local[36],write=0,cursor,field,position;
    uint16_t kind;
    owner=B(0x799a99u+offset);kind=W(0x7999e8u+offset);
    if(kind==72u||kind==193u){
        cursor=0x799ab8u+offset;position=U(cursor);
        if(position==0u)U(cursor)=1;
        else if((int32_t)position>14)U(cursor)=U(0xf53e6cu)-13u;
        position=U(cursor);ordinal=U(0xd3b198u);
        resource=U(0xf53e68u+(position+ordinal)*4u);
    }else if(kind==70u||kind==190u){
        cursor=0xf53e70u;
        do{
            if(FUN_005013a0(U(cursor)))local[write++]=U(cursor);
            cursor+=4u;
        }while((int32_t)cursor<0xf53f00);
        ordinal=U(0xd3b198u);building=U(0xeadd6cu);
        resource=local[ordinal-1u];
    }else{
        ordinal=U(0xd3b198u);resource=U(0xf53dd8u+ordinal*4u);
    }
    field=0x7900c2u+owner*196u+resource;
    switch(B(field)){
    case 0:
        kind=W(0x7999e8u+building*264u);
        B(field)=kind==70u||kind==190u?1u:2u;break;
    case 1:B(field)=0;break;
    case 2:B(field)=3;break;
    case 3:B(field)=1;break;
    default:break;
    }
    B(0xe38dcau)=1;
}
/* Native406460..406615:123 instructions. The two buttons for each of
   the30 resource rows publish action1/decrease or0/increase and a zero-based
   row at5C81F8. Policies other than zero use the second signed WORD limit.
   Keep native write ordering: some exits redraw before the limit store. */
void restore_storage_adjust_limit_00406460(void)
{
    uint32_t building=U(0xeadd6cu),offset=building*264u;
    uint32_t owner=B(0x799a99u+offset),position=U(0x799ab8u+offset);
    uint32_t resource,action,policy,limit_at,redraw_first;
    int32_t limit;
    uint16_t kind;
    if(position==0u)U(0x799ab8u+offset)=1;
    else{
        position=FUN_00502780();
        if((int32_t)U(0x799ab8u+offset)>(int32_t)position){
            building=U(0xeadd6cu);offset=building*264u;
            position=FUN_00502780();
            U(0x799ab8u+offset)=U(0xf53e6cu)-position+1u;
        }
    }
    kind=W(0x7999e8u+offset);
    if(kind==72u||kind==193u){
        position=U(0x799ab8u+offset)+U(0xd3b194u);
        resource=U(0xf53e6cu+position*4u);
    }else resource=U(0xf53ddcu+U(0xd3b194u)*4u);
    action=U(0xd3b198u);
    if(action==0u||action==1u){
        policy=B(0x7900c2u+owner*196u+resource);
        limit_at=(policy==0u?0x7900ecu:0x790134u)+owner*196u+resource*2u;
        limit=(int32_t)(int16_t)W(limit_at);
        redraw_first=policy==0u;
        if(action==1u){
            limit-=800;
            if(limit<800){limit=800;redraw_first=1;}
        }else{
            limit+=800;
            if(limit>3200)limit=3200;
        }
        if(redraw_first){B(0xe38dcau)=1;W(limit_at)=(uint16_t)limit;}
        else{W(limit_at)=(uint16_t)limit;B(0xe38dcau)=1;}
    }else B(0xe38dcau)=1;
}
void FUN_00406460(void)
{restore_storage_adjust_limit_00406460();}

int restore_storage_order_action(uint32_t token)
{
    if(token==0x406300u){storage_toggle();return 1;}
    if(token==0x406460u){restore_storage_adjust_limit_00406460();return 1;}
    return 0;
}
