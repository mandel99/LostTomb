#ifndef RESTORE_MENU_INPUT_H
#define RESTORE_MENU_INPUT_H
#include <windows.h>
#include <stdint.h>
#include <stddef.h>
typedef struct RestoreMenuButton {
    int16_t left,top,right,bottom;
    uint32_t on_left,on_right;
    uint8_t reserved16,activated,reserved18,trigger;
    int32_t primary,secondary;
} RestoreMenuButton;
typedef char restore_menu_button_size[sizeof(RestoreMenuButton)==28?1:-1];
typedef char restore_menu_button_trigger[offsetof(RestoreMenuButton,trigger)==19?1:-1];
typedef struct RestoreSmallButton {
    int16_t left,top,width,height,kind,image;
    uint32_t reserved12,on_left,on_right;
    uint8_t enabled,pressed,hover,locked;
    int32_t primary,secondary;
} RestoreSmallButton;
typedef char restore_small_button_size[sizeof(RestoreSmallButton)==36?1:-1];
int FUN_004261d0(int x,int y,RestoreMenuButton *buttons,int count);
int FUN_00426450(int x,int y,const RestoreMenuButton *buttons,int count);
void restore_menu_callback(uint32_t address);
BOOL restore_input_cursor(POINT *position);
void FUN_004cf210(void);
typedef struct RestoreMenuActionApi {
    void (*game)(uint32_t address);
    BOOL (WINAPI *show)(HWND,int);
    HINSTANCE (WINAPI *open)(HWND,LPCSTR,LPCSTR,LPCSTR,LPCSTR,INT);
    BOOL (WINAPI *post)(HWND,UINT,WPARAM,LPARAM);
} RestoreMenuActionApi;
const RestoreMenuActionApi *restore_menu_action_platform(void);
void restore_main_menu_action(const RestoreMenuActionApi *api);
void FUN_00417ff0(void);
#endif
