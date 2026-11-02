#include "menu_input.h"
#include <shellapi.h>
extern void FUN_00516bb0(void);
extern void FUN_0041f870(void);
extern void FUN_004c5920(void);
static void game(uint32_t address)
{
    switch(address) {
    case 0x00516bb0: FUN_00516bb0();break;
    case 0x0041f870: FUN_0041f870();break;
    case 0x004c5920: FUN_004c5920();break;
    }
}
static const RestoreMenuActionApi api={game,ShowWindow,ShellExecuteA,PostMessageA};
const RestoreMenuActionApi *restore_menu_action_platform(void) {return &api;}
BOOL restore_input_cursor(POINT *position) {return GetCursorPos(position);}
