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
int restore_storage_order_action(uint32_t token)
{
    if(token!=0x406300u)return 0;
    storage_toggle();return 1;
}
