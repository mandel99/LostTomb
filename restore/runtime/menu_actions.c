#include "menu_input.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
void restore_main_menu_action(const RestoreMenuActionApi *api)
{
    uint32_t action=U(0x00d3b198);
    B(0x00e38ea8)=0;U(0x005d2a2c)=1;
    switch(action) {
    case 0: api->game(0x00516bb0);U(0x00f53f78)=0;break;
    case 1:
        api->show(*(HWND *)0x00e39140u,SW_MINIMIZE);
        api->open(*(HWND *)0x00e39140u,(LPCSTR)0x005761d8u,(LPCSTR)0x0057bfb0u,0,0,SW_SHOW);
        break;
    case 2:
        U(0x00e38ebc)=2;U(0x00605a3c)=1;B(0x00e38e60)=0;
        U(0x00f53f78)=0;U(0x00704db0)=0;U(0x00704dac)=3;
        api->game(0x0041f870);break;
    case 3: api->game(0x004c5920);B(0x00e38dda)=0x53;break;
    case 4: api->post(*(HWND *)0x00e39140u,WM_KEYDOWN,VK_ESCAPE,0);break;
    }
}
void FUN_00417ff0(void) {restore_main_menu_action(restore_menu_action_platform());}
