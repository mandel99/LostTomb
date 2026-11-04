#include "owned_buffer.h"
#include "host_allocator.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
RestoreOwnedBuffer *restore_owned_buffer_initialize(RestoreOwnedBuffer *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self;
    U(base+4u)=0;U(base)=0;
    return self;
}
void restore_owned_buffer_release(RestoreOwnedBuffer *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self;
    if(U(base+4u)&&U(base)){
        FUN_00561bdc((void *)(uintptr_t)U(base));
        U(base)=0;
    }
}
