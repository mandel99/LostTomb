#include "owned_buffer.h"
#include "array_lifecycle.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00561a2b(void *allocation);
void restore_owned_buffer_construct_element(void *self)
{(void)FUN_00560280((RestoreOwnedBuffer *)self);}
void restore_owned_buffer_destroy_element(void *self)
{FUN_005602b0((RestoreOwnedBuffer *)self);}
void *restore_owned_buffer_delete(RestoreOwnedBuffer *self,uint32_t flags)
{
    uint32_t base=(uint32_t)(uintptr_t)self;
    if(flags&2u){
        FUN_00561363(self,8,(int32_t)U(base-4u),restore_owned_buffer_destroy_element);
        if(flags&1u)FUN_00561a2b((void *)(uintptr_t)(base-4u));
        return (void *)(uintptr_t)(base-4u);
    }
    FUN_005602b0(self);
    if(flags&1u)FUN_00561a2b(self);
    return self;
}
void restore_owned_buffer_cleanup_frame(uint32_t frame)
{FUN_005602b0((RestoreOwnedBuffer *)(uintptr_t)(frame-20u));}
