#include "legacy_type_info.h"
extern void FUN_00561bdc(void *allocation);

/* Original 563320: install type_info vtable and release the cached name. */
void FUN_00563320(restore_type_info *object)
{
    object->vtable = restore_type_info_vftable;
    if (object->cached_name != 0)
        FUN_00561bdc(object->cached_name);
}
