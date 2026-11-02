#include "dropdown_system_actions.h"
#include "dropdown_menu.h"
#include "message_panel.h"
#include "menu_input.h"

void FUN_00402b90(void)
{
    const RestoreMenuActionApi *api=restore_menu_action_platform();
    /* Original queues Escape; the window's own quit/confirmation flow follows. */
    api->post((HWND)(uintptr_t)DM_U(0xe39140),WM_KEYDOWN,VK_ESCAPE,0);
}
void FUN_004033f0(void) {FUN_004c5bf0(10,0,0);}
void FUN_00403400(void) {FUN_004c5bf0(0,0,0);}
void FUN_004034d0(void)
{
    const RestoreMenuActionApi *api=restore_menu_action_platform();
    api->show((HWND)(uintptr_t)DM_U(0xe39140),SW_MINIMIZE);
    api->open((HWND)(uintptr_t)DM_U(0xe39140),(LPCSTR)0x5761d8u,
              (LPCSTR)0x5761e0u,0,0,SW_SHOW);
}
int restore_dropdown_system_action(uint32_t token)
{
    switch(token){
    case 0x402b90:FUN_00402b90();break;
    case 0x4033f0:FUN_004033f0();break;
    case 0x403400:FUN_00403400();break;
    case 0x4034d0:FUN_004034d0();break;
    default:return 0;
    }
    return 1;
}
