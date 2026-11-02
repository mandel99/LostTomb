#include "menu_cursor.h"
static HCURSOR WINAPI set(HCURSOR cursor){return SetCursor(cursor);}
static const RestoreMenuCursorApi api={set};
const RestoreMenuCursorApi *restore_menu_cursor_platform(void){return &api;}
