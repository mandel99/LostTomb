#include "editor_popup_actions.h"
#include "selection_popup.h"
#include "menu_dialog.h"
#include "map_commit_state.h"
#include "mission_unlock.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)(int16_t)W(a))

/* Whole4022D0..402331. Actual confirmation callback, no arguments or
   incoming receiver. Reload selected after the real financial operation. */
static void confirm_004022d0(void)
{
    uint32_t selected,player;int32_t amount;
    if(B(0xd3b439u)!=1u)return;
    selected=U(0x784f18u);player=U(0xeaac28u);
    amount=S(0x77de4eu+selected*106u);
    FUN_004e9680((int32_t)player,amount);
    selected=U(0x784f18u);
    B(0x77de05u+selected*106u)=1;
    FUN_00404350();FUN_004051a0();
    U(0x70570cu)=0x26;
    B(0xe38dcau)=1;
}
/* Whole409490..40950B. The unsigned BYTE threshold and signed funds guard
   are native; both confirmation branches use the actual callback above. */
static void action_00409490(void)
{
    uint32_t selected=U(0x784f18u),record=selected*106u,player,route;
    if(B(0x77de05u+record)>=50u)return;
    player=U(0xeaac28u);
    if((int32_t)U(0xea187cu+player*0x49d8u)<=-5000){
        FUN_004264d0(0x77u,0x51f780u,0);return;
    }
    route=B(0x77de04u+record);
    FUN_004264d0(B(0x7770f0u+route*324u)==1u?2u:4u,0x4022d0u,2);
}
int restore_editor_popup_action(uint32_t token)
{
    switch(token){
    case 0x4022d0u:confirm_004022d0();return 1;
    case 0x409490u:action_00409490();return 1;
    case 0x409200u:
        FUN_00403c00(20,40,11,32,0,0,0x784b62u,0,0,2,0,0,0,-1,0);
        B(0xe38dcau)=1;return 1;
    case 0x409240u:{
        uint32_t alternate=U(0x605a54u);
        U(0x605a58u)=1;
        FUN_00403c00(0,0,alternate?62:63,151,alternate?63u:0u,
                     0,0,0x784f08u,0,4,0,0,0,22,0);
        B(0xe38dcau)=1;return 1;
    }
    case 0x409350u:
        FUN_00403c00(0,0,31,152,0,0,0,0x784f0cu,0,4,0,0,0,16,0);
        B(0xe38dcau)=1;return 1;
    default:return 0;
    }
}
