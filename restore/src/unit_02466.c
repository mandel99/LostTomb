#include "menu_keyboard_modal.h"
int FUN_00538cd0(uint32_t title,uint32_t body,uint32_t flags)
{
    const RestoreModalEventApi *api=restore_menu_modal_events();
    uint8_t old_mode=MM_B(0xe38dda);int selected=0,done=0;MSG message;
    MM_B(0xe38dda)=0x54;MM_B(0xf535c0)=0;
    do{
        if(api->peek(&message,NULL,0,0,0)){
            if(!api->get(&message,NULL,0,0))break;
            api->translate(&message);api->dispatch(&message);
        }else if(!MM_U(0xe39128)&&MM_U(0xe3912c)){
            selected=FUN_00538c80(title,body,flags);done=selected!=-1;
        }else api->wait();
        if(MM_B(0xd3b43d)==1){
            uint32_t window=MM_U(0xe39140);
            (void)restore_menu_action_platform()->post((HWND)(uintptr_t)window,WM_CLOSE,0,0);
            MM_B(0xd3b43d)=0;
        }
    }while(!done);
    MM_B(0xe38dda)=old_mode;MM_B(0xe38dca)=1;return selected;
}
