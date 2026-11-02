#include "window_messages.h"
#include "startup_trace.h"
LRESULT CALLBACK restore_window_proc(HWND window,UINT message,WPARAM key,LPARAM value)
{
    char enabled[8];
    if(((message>=WM_KEYFIRST&&message<=WM_KEYLAST)||(message>=WM_MOUSEFIRST&&message<=WM_MOUSELAST))&&
       GetEnvironmentVariableA("RESTORE_INTERNAL_INPUT_ONLY",enabled,sizeof(enabled))==1&&enabled[0]=='1'){
        static int reported;
        if(!reported){restore_startup_trace("STARTUP diagnostic external window input ignored; internal fixture input remains active\n");reported=1;}
        return 0;
    }
    return restore_window_message(restore_window_platform(),window,message,key,value);
}
