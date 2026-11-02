#include "menu_keyboard_modal.h"
static BOOL WINAPI peek(LPMSG message,HWND window,UINT low,UINT high,UINT remove){return PeekMessageA(message,window,low,high,remove);}
static BOOL WINAPI get(LPMSG message,HWND window,UINT low,UINT high){return GetMessageA(message,window,low,high);}
static BOOL WINAPI translate(const MSG *message){return TranslateMessage(message);}
static LRESULT WINAPI dispatch(const MSG *message){return DispatchMessageA(message);}
static BOOL WINAPI wait(void){return WaitMessage();}
static const RestoreModalEventApi api={peek,get,translate,dispatch,wait};
const RestoreModalEventApi *restore_menu_modal_events(void){return &api;}
