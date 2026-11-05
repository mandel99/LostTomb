#include "placement_destructor.h"
/* Receiver-only canonical ABI bridge. Native RET has no stack arguments. */
__declspec(naked) void FUN_0040d780(void)
{
    __asm {
        push ecx
        call restore_placement_destroy_0040d780
        add esp,4
        ret
    }
}
