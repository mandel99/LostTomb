#include "menu_keyboard_modal.h"
extern void FUN_004cf210(void);
extern void FUN_004d71c0(int redraw);
extern void FUN_004d8310(void);
extern void FUN_004d0280(int flags);
extern void FUN_004cf1f0(void);
extern void FUN_004cf460(uint32_t cursor);
int FUN_00538c80(uint32_t title,uint32_t body,uint32_t flags)
{
    int result;
    FUN_004cf210();FUN_004d71c0(0);
    result=FUN_00538940(title,body,flags);
    FUN_004d8310();FUN_004d0280(0);FUN_004cf1f0();FUN_004cf460(0);
    return result;
}
