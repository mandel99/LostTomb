#include "event_parameters.h"
__declspec(naked) void FUN_004487f0(void)
{
    __asm { push ecx }
    __asm { push eax }
    __asm { push ecx }
    __asm { call restore_event_parameter_choose }
    __asm { add esp,8 }
    __asm { pop ecx }
    __asm { ret }
}
