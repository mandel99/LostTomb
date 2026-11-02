#ifndef RESTORE_LEGACY_TYPE_INFO_H
#define RESTORE_LEGACY_TYPE_INFO_H
#include <stdint.h>
typedef struct restore_type_info {
    const void *vtable;
    void *cached_name;
} restore_type_info;
extern const uintptr_t restore_type_info_vftable[1];
void FUN_00563320(restore_type_info *object);
restore_type_info *FUN_00563335(restore_type_info *object, unsigned char flags);
#endif
