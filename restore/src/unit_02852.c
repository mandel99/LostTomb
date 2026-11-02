#include "legacy_type_info.h"
extern void FUN_00561a2b(unsigned int allocation);

restore_type_info *FUN_00563335(restore_type_info *object, unsigned char flags)
{
    FUN_00563320(object);
    if ((flags & 1) != 0)
        FUN_00561a2b((unsigned int)(uintptr_t)object);
    return object;
}

/* ECX carries this; the single stack argument is callee-cleaned (ret 4). */
static restore_type_info *__fastcall deleting_destructor(
    restore_type_info *object, void *unused_edx, unsigned int flags)
{
    (void)unused_edx;
    return FUN_00563335(object, (unsigned char)flags);
}
const uintptr_t restore_type_info_vftable[1] = {(uintptr_t)deleting_destructor};
