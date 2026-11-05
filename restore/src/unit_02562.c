#include "placement_destructor.h"
/* Receiver-only canonical ABI bridge. Native RET has no stack arguments. */
__declspec(naked) void FUN_00545630(void)
{
    __asm {
        push ecx
        call restore_placement_destroy_00545630
        add esp,4
        ret
    }
}
