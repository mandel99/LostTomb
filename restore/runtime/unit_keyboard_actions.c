#include "unit_keyboard_actions.h"
#include "unit_button_actions.h"
#include "menu_list_scroll.h"
#include "map_commit_links.h"
#include "actor_walking.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)(int16_t)W(a))
static uint32_t actor(uint32_t id){return 0xbcd5f8u+id*388u;}

/* Whole 4BC670,42 instructions,cdecl1/RET0. The argument actor is changed,
   but the native walking call intentionally uses fresh global D056F8.
   Preserve both full-DWORD neighbor-query outputs across that operation. */
void restore_unit_actor_return_004bc670(uint32_t id)
{
    uint32_t old_y=U(0xd3b4e0u),a=actor(id),old_x=U(0xd3b4e4u),b,layout,walking;
    int32_t building,x,y;uint16_t previous_x,previous_y,target_x,target_y;
    B(a+56u)=11;building=S(a+90u);b=0x7999d8u+(uint32_t)building*264u;
    layout=B(b+3u);y=S(b+8u);x=S(b+6u);
    (void)FUN_004806c0(x,y,layout);
    previous_x=W(a+20u);target_x=W(0xd3b4e4u);W(a+44u)=previous_x;
    previous_y=W(a+22u);target_y=W(0xd3b4e0u);W(a+46u)=previous_y;
    walking=U(0xd056f8u);W(a+36u)=target_x;W(a+38u)=target_y;
    U(0xd3b4e4u)=old_x;U(0xd3b4e0u)=old_y;FUN_004b3250((int32_t)walking);
}

/* Whole 406C90,75 instructions,including four exact targets at406DCC.
   The lock gate precedes even the selected-ID read and D05704 write. */
void restore_unit_keyboard_00406c90(void)
{
    uint32_t id,a,maximum,value,mode;int32_t percentage;uint8_t state;
    if(B(0xe38e60u)!=0u)return;
    id=U(0xeadcc4u);U(0xd05704u)=id;if((int32_t)id<=0)return;
    a=actor(id);maximum=U(a+300u);value=U(a+296u);
    percentage=FUN_004cde60((int32_t)value,(int32_t)maximum);state=B(a+56u);
    if((state==13u||state==15u)&&percentage<=15)return;
    mode=U(0xd3b198u);
    if(mode==3u&&(state==13u||state==15u||percentage==100))return;
    if(S(a+306u)>=0)return;
    switch(mode){
    case 0:
        B(a+56u)=20;U(0xd0570cu)=1;B(0xe38ddau)=32;B(0xe38dcau)=1;break;
    case 1:
        B(a+56u)=23;U(a+348u)=22;U(0xd0570cu)=1;
        B(0xe38ddau)=32;B(0xe38dcau)=1;break;
    case 2:
        B(a+56u)=10;U(a+348u)=16;U(0xd0570cu)=0;
        B(0xe38ddau)=1;B(0xe38dcau)=1;break;
    case 3:
        U(0xd0570cu)=0;restore_unit_actor_assign_building(id);
        B(0xe38ddau)=32;B(0xe38dcau)=1;break;
    default:break;
    }
}

/* Whole 406DE0,46 instructions. No D05704 assignment in this callback.
   4A4300's consumed AL and the subsequent fresh mode read are explicit. */
void restore_unit_keyboard_00406de0(void)
{
    uint32_t id=U(0xeadcc4u),a,maximum,value;int32_t percentage;uint8_t state;
    if((int32_t)id<=0||B(0xe38e60u)!=0u)return;
    a=actor(id);maximum=U(a+300u);value=U(a+296u);
    percentage=FUN_004cde60((int32_t)value,(int32_t)maximum);state=B(a+56u);
    if((state==13u||state==15u)&&percentage<=15)return;
    if(U(0xd3b198u)==4u&&restore_unit_actor_is_linked(id)!=0u)return;
    if(S(a+306u)>=0)return;
    if(U(0xd3b198u)==4u){U(0xd0570cu)=0;restore_unit_actor_return_004bc670(id);}
    B(0xe38ddau)=1;B(0xe38dcau)=1;
}

int restore_unit_keyboard_action(uint32_t token)
{
    switch(token){
    case 0x406c90u:restore_unit_keyboard_00406c90();return 1;
    case 0x406de0u:restore_unit_keyboard_00406de0();return 1;
    default:return 0;
    }
}
