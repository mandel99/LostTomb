#include "legacy_member_calls.h"
/* Zero-stack-argument use of the original pop/pop/xchg/jmp member thunk. */
void FUN_005614f7(void *object, uintptr_t target)
{
    restore_call_member0(object, target);
}
