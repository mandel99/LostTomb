#include "window_messages.h"
#include <stdio.h>
#include <stdlib.h>
static RestoreWindowApi api;
static unsigned calls;
static HWND last_window;static UINT last_message;static WPARAM last_key;static LPARAM last_value;
const RestoreWindowApi *restore_window_platform(void){return &api;}
LRESULT restore_window_message(const RestoreWindowApi *p,HWND w,UINT m,WPARAM k,LPARAM v)
{if(p!=&api)abort();++calls;last_window=w;last_message=m;last_key=k;last_value=v;return 123;}
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Diagnostic input isolation failed at%d\n",__LINE__);return 1;}}while(0)
int main(void)
{
    const UINT inputs[]={WM_KEYDOWN,WM_KEYUP,WM_CHAR,WM_SYSKEYDOWN,WM_MOUSEMOVE,WM_LBUTTONDOWN,WM_LBUTTONUP,WM_RBUTTONDOWN,WM_RBUTTONUP,WM_MOUSEWHEEL};
    unsigned i;char previous[32768];DWORD length=GetEnvironmentVariableA("RESTORE_INTERNAL_INPUT_ONLY",previous,sizeof(previous));
    CHECK(length<sizeof(previous));CHECK(SetEnvironmentVariableA("RESTORE_INTERNAL_INPUT_ONLY",NULL));
    CHECK(restore_window_proc((HWND)17,WM_LBUTTONUP,2,3)==123&&calls==1&&last_window==(HWND)17&&last_message==WM_LBUTTONUP&&last_key==2&&last_value==3);
    CHECK(SetEnvironmentVariableA("RESTORE_INTERNAL_INPUT_ONLY","1"));
    for(i=0;i<sizeof(inputs)/sizeof(inputs[0]);++i)CHECK(restore_window_proc((HWND)17,inputs[i],2,3)==0&&calls==1);
    CHECK(restore_window_proc((HWND)17,WM_PAINT,2,3)==123&&calls==2);
    /* The fixture calls the same real dispatcher directly, after host polling. */
    CHECK(restore_window_message(restore_window_platform(),(HWND)17,WM_CHAR,'R',0)==123&&calls==3&&last_key=='R');
    CHECK(SetEnvironmentVariableA("RESTORE_INTERNAL_INPUT_ONLY","0"));
    CHECK(restore_window_proc((HWND)17,WM_KEYDOWN,'A',3)==123&&calls==4);
    CHECK(SetEnvironmentVariableA("RESTORE_INTERNAL_INPUT_ONLY",length?previous:NULL));
    puts("Diagnostic input: opt-in external event filter, live paint and internal fixture dispatch.");return 0;
}
