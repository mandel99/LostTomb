#include "placement_destructor.h"
/* Receiver-only canonical ABI bridge. Native RET has no stack arguments. */
__declspec(naked) void FUN_00555d70(void)
{
    __asm {
        push ecx
        call restore_placement_destroy_00555d70
        add esp,4
        ret
    }
}
