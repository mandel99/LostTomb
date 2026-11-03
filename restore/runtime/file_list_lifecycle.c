#include "file_list.h"
#include "host_allocator.h"

RestoreFileList *restore_file_list_construct(RestoreFileList *object)
{
    volatile RestoreFileList *self=object;
    self->legacy_vtable=0x56f548u;
    self->offsets=0;self->strings=0;
    self->count=0;self->capacity=0;self->string_capacity=0;self->used=0;
    return object;
}
RestoreFileList *restore_file_list_construct_mission(RestoreFileList *object)
{
    (void)restore_file_list_construct(object);
    ((volatile RestoreFileList *)object)->legacy_vtable=0x56f54cu;
    return object;
}
void restore_file_list_destroy(RestoreFileList *object)
{
    volatile RestoreFileList *self=object;
    /* Native captures strings BEFORE replacing the vtable, and reloads
       offsets AFTER freeing strings. Keep callback-sensitive ownership order.
       It does not clear either freed pointer or any size/count field. */
    char *strings=self->strings;
    uint32_t *offsets;
    self->legacy_vtable=0x56f548u;
    if(strings)restore_heap_free(strings);
    offsets=self->offsets;
    if(offsets)restore_heap_free(offsets);
}
RestoreFileList *restore_file_list_delete(RestoreFileList *self,uint32_t flags)
{
    uintptr_t address=(uintptr_t)self;
    restore_file_list_destroy(self);
    if(flags&1u)restore_heap_free((void *)address);
    /* The native deleting destructor returns the old address, including
       after deallocation. Preserve its integer bits without dereferencing it. */
    return (RestoreFileList *)address;
}
void restore_file_list_destroy_frame(uint32_t frame,uint32_t distance)
{restore_file_list_destroy((RestoreFileList *)(uintptr_t)(frame-distance));}

/* Original CRT entries576008/576034/576074 and their exit callbacks. */
int FUN_00402360(void){(void)FUN_00402370();return FUN_00402380();}
void FUN_00402390(void){restore_file_list_destroy(RESTORE_SAVE_FILES);}
int FUN_0041e050(void){(void)FUN_0041e060();return FUN_0041e070();}
void FUN_0041e080(void){restore_file_list_destroy(RESTORE_MISSION_FILES);}
int FUN_00516570(void){(void)FUN_00516580();return FUN_00516590();}
void FUN_005165a0(void){restore_file_list_destroy(RESTORE_FAMILY_FILES);}
