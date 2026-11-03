#include "event_schedule.h"
__declspec(naked) void FUN_0044c430(void)
{
 __asm { push eax }
 __asm { push dword ptr[esp+8] }
 __asm { push ecx }
 __asm { call restore_event_dispatch_deferred }
 __asm { add esp,12 }
 __asm { ret }
}
