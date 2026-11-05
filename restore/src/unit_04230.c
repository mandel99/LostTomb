#include "placement_destructor.h"
/* Pure EH ABI adapter: EBP belongs to the original parent, not this C function. */
__declspec(naked) void FUN_0056dc00(void)
{
    __asm {
        mov ecx,dword ptr[ebp-0x44]
        jmp FUN_00545fa0
    }
}
