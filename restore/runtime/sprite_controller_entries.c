/* Canonical Win32 ABI entries. Keep canonical ownership here separate from
   the named-only original-file candidates. Native RET cleanup is retained.
   EDX is dead on entry to 55D330 before its first use. The 55D840 bridge
   exposes the observed WORD result; EAX high16 is outside that API. */
#include "sprite_controller.h"

__declspec(naked) void FUN_0055d330(void)
{
    __asm { lea edx,[esp+4] }
    __asm { push edx }
    __asm { push dword ptr[esp+10h] }
    __asm { push dword ptr[esp+0ch] }
    __asm { push ecx }
    __asm { call restore_sprite_cycle_select_action }
    __asm { add esp,16 }
    __asm { ret 12 }
}
__declspec(naked) void FUN_0055d460(void)
{
    __asm { push dword ptr[esp+4] }
    __asm { push ecx }
    __asm { call restore_sprite_cycle_set_action }
    __asm { add esp,8 }
    __asm { ret 4 }
}
__declspec(naked) void FUN_0055d810(void)
{
    __asm { push ecx }
    __asm { call restore_sprite_cycle_next_action }
    __asm { add esp,4 }
    __asm { ret }
}
__declspec(naked) void FUN_0055d6d0(void)
{
    __asm { push ecx }
    __asm { call restore_sprite_cycle_dispatch }
    __asm { add esp,4 }
    __asm { ret }
}
__declspec(naked) void FUN_0055d840(void)
{
    __asm { push ecx }
    __asm { call restore_sprite_cycle_direction }
    __asm { add esp,4 }
    __asm { ret }
}
