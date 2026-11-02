#ifndef RESTORE_WINDOW_MESSAGES_H
#define RESTORE_WINDOW_MESSAGES_H
#include <windows.h>
#include <stdint.h>
/* Game-entry numbers are dispatch identifiers, never callable addresses. The
   production adapter switches to actual compiled C symbols. */
typedef struct RestoreWindowApi {
    LRESULT (WINAPI *def_window)(HWND,UINT,WPARAM,LPARAM);
    BOOL (WINAPI *is_iconic)(HWND);
    HWND (WINAPI *foreground)(void);
    HWND (WINAPI *set_active)(HWND);
    BOOL (WINAPI *set_foreground)(HWND);
    BOOL (WINAPI *show)(HWND,int);
    int (WINAPI *metrics)(int);
    BOOL (WINAPI *client_rect)(HWND,LPRECT);
    BOOL (WINAPI *client_to_screen)(HWND,LPPOINT);
    LONG (WINAPI *window_long)(HWND,int);
    HMENU (WINAPI *menu)(HWND);
    BOOL (WINAPI *adjust)(LPRECT,DWORD,BOOL,DWORD);
    BOOL (WINAPI *position)(HWND,HWND,int,int,int,int,UINT);
    HDC (WINAPI *begin_paint)(HWND,LPPAINTSTRUCT);
    BOOL (WINAPI *end_paint)(HWND,const PAINTSTRUCT *);
    void (WINAPI *quit)(int);
    BOOL (WINAPI *post)(HWND,UINT,WPARAM,LPARAM);
    LONG (WINAPI *message_time)(void);
    DWORD (WINAPI *message_pos)(void);
    uintptr_t (*game)(uint32_t,uintptr_t,uintptr_t,uintptr_t);
    BOOL (WINAPI *invalidate)(HWND,const RECT *,BOOL);
} RestoreWindowApi;
LRESULT restore_window_message(const RestoreWindowApi *api,HWND window,UINT message,WPARAM key,LPARAM value);
const RestoreWindowApi *restore_window_platform(void);
LRESULT CALLBACK restore_window_proc(HWND window,UINT message,WPARAM key,LPARAM value);
#endif
