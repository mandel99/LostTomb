#include "sprite_tag_list.h"
/* Forward the real native ECX and node stack slot; native callee pops4. */
__declspec(naked) void FUN_005404b0(void)
{
    __asm {
        push dword ptr [esp+4]
        push ecx
        call restore_sprite_tag_append
        add esp,8
        ret 4
    }
}
