#include "display_confirmation.h"
void FUN_00402de0(void)
{
    uint32_t window=DC_U(0xe39140);uint8_t mode;
    (void)restore_menu_action_platform()->post((HWND)(uintptr_t)window,WM_KEYDOWN,VK_F7,0);
    mode=DC_B(0x70570c);DC_B(0xe38dca)=1;DC_B(0xe38dda)=mode;
}
