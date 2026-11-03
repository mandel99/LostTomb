#include "event_schedule.h"
__declspec(naked) void FUN_0044bc20(void)
{
 __asm { push ecx }
 __asm { call restore_event_rearm_completed }
 __asm { add esp,4 }
 __asm { ret }
}
