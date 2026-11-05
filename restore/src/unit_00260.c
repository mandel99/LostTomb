#include "campaign_unlock.h"
/* Actual native member receiver; no stack arguments, native RET. */
__declspec(naked) void FUN_0041bae0(void)
{
    __asm {
        push ecx
        call restore_campaign_unlock_all
        add esp,4
        ret
    }
}
