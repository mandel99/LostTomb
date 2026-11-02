#include "menu_cursor.h"
HCURSOR restore_menu_cursor_select(uint32_t selector)
{
    static const uint32_t slots[19]={
        0xe390d8,0xe390b8,0xe390d0,0xe390cc,0xe390c4,
        0xe390c8,0xe39094,0xe390bc,0xe390d4,0xe390b4,
        0xe390b0,0xe390a8,0xe390ac,0xe390a0,0xe390a4,
        0xe3909c,0xe39098,0xe390c0,0xe39090
    };
    uint32_t address=slots[selector<=18u?selector:0];
    HCURSOR cursor=*(HCURSOR *)(uintptr_t)address;
    *(HCURSOR *)0xe390dcu=cursor;
    return restore_menu_cursor_platform()->set(cursor);
}
void FUN_004cf460(uint32_t selector)
{
    (void)restore_menu_cursor_select(selector);
}
