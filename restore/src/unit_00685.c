#include "event_parameters.h"
__declspec(naked) void FUN_00448a90(void)
{
    __asm { push ecx }
    __asm { call restore_event_record_reset }
    __asm { add esp,4 }
    __asm { mov ecx,0 }
    __asm { ret }
}
