#include <stdint.h>
extern void FUN_004454a0(void);
/* Native EH cleanup56B650: the receiver is genuinely an EBP-relative local
   in the parent SEH frame. Keep only that context adaptation in assembly;
   the destructor's allocation branch and writes are the shared C body. */
__declspec(naked) void FUN_0056b650(void)
{
    __asm { lea ecx,[ebp-150h] }
    __asm { jmp FUN_004454a0 }
}
