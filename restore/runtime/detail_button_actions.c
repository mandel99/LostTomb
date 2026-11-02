#include "detail_button_actions.h"
#include "menu_group_selection.h"
#include "message_panel.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Complete4071C0,36 instructions; exact PE mapper407278[15] selects four
   targets407211/40722B/407249/407253. Unknown orders still call406720,
   retaining the old DWORD action. Read-only state exits without dirty stores. */
void FUN_004071c0(void)
{
    uint32_t group;int32_t order;
    if(U(0xeadcc0u)!=0){U(0xeadcc0u)=0;B(0xe38dcau)=1;return;}
    if(U(0xeadd50u)==4){
        if(B(0xe38e60u)!=0)return;
        group=U(0xbcb9c0u);
        order=H(0xbcb9f2u+group*144u);
        switch(order){
        case 1:case 2:U(0xd3b198u)=0;break;
        case 3:case 4:U(0xd3b198u)=1;break;
        case 15:U(0xd3b198u)=2;break;
        default:break;
        }
        FUN_00406720();
    }
    B(0xe38ddau)=1;
    B(0xe38dcau)=1;
}
/* Complete407290,13 instructions. The signed1..14 check is native.
   Its immutable DWORD table576148[15] contains message IDs, not callbacks. */
void FUN_00407290(void)
{
    uint32_t selected=U(0x605a44u),message;
    if((int32_t)selected<=0||(int32_t)selected>14)return;
    message=U(0x576148u+selected*4u);
    FUN_004c5bf0((int32_t)message,1,0);
    B(0xe38dcau)=1;
}
/* Complete407320,8 instructions. The real button handler publishes the row's
   secondary DWORD toD3B194 before invoking this right-click callback. */
void FUN_00407320(void)
{
    uint32_t message=U(0xd3b194u);
    FUN_004c5bf0((int32_t)message,0,0);
    B(0xe38dcau)=1;
}
int restore_detail_button_action(uint32_t token)
{
    switch(token){
    case 0x4071c0u:FUN_004071c0();break;
    case 0x407290u:FUN_00407290();break;
    case 0x407320u:FUN_00407320();break;
    default:return 0;
    }
    return 1;
}
