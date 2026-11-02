#include "legacy_type_info.h"
#include <stdio.h>
static void *name_freed;
static uintptr_t object_freed;
static unsigned int name_frees, object_frees;
/* Test allocator observations; production keeps the original allocator pair. */
void FUN_00561bdc(void *allocation) {name_freed=allocation; ++name_frees;}
void FUN_00561a2b(unsigned int allocation) {object_freed=allocation; ++object_frees;}
int main(void)
{
    restore_type_info object;
    int cached_name;
    typedef restore_type_info *(__fastcall *DeletingDestructor)(restore_type_info *, void *, unsigned int);
    DeletingDestructor destroy=(DeletingDestructor)restore_type_info_vftable[0];
    object.vtable=0; object.cached_name=0;
    FUN_00563320(&object);
    if (object.vtable!=restore_type_info_vftable || name_frees || object_frees) return 1;
    object.cached_name=&cached_name;
    if (FUN_00563335(&object,0)!=&object || name_freed!=&cached_name || name_frees!=1 || object_frees) return 2;
    if (destroy(&object,0,1)!=&object || name_frees!=2 || object_frees!=1 || object_freed!=(uintptr_t)&object) return 3;
    object.cached_name=0;
    if (destroy(&object,0,2)!=&object || name_frees!=2 || object_frees!=1) return 4;
    puts("type_info: cached-name release, delete flags, vtable ABI and return verified.");
    return 0;
}
