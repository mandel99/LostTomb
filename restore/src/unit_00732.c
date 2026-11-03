#include "menu_event_actions.h"
__declspec(naked) void FUN_0044c490(void)
{
 __asm { push dword ptr[esp+8] }
 __asm { push dword ptr[esp+8] }
 __asm { push ecx }
 __asm { call restore_event_schedule_0044c490 }
 __asm { add esp,12 }
 __asm { ret }
}
