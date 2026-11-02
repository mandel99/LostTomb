#ifndef RESTORE_MENU_CURSOR_H
#define RESTORE_MENU_CURSOR_H
#include <windows.h>
#include <stdint.h>
typedef struct RestoreMenuCursorApi {
    HCURSOR (WINAPI *set)(HCURSOR cursor);
} RestoreMenuCursorApi;
typedef char restore_cursor_handle_width[sizeof(HCURSOR)==4?1:-1];
const RestoreMenuCursorApi *restore_menu_cursor_platform(void);
HCURSOR restore_menu_cursor_select(uint32_t selector);
void FUN_004cf460(uint32_t selector);
#endif
