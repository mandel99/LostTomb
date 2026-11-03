#include "actor_ring_queue.h"

/* CRT entry576070 ->4F64A0. These field-only constructors cannot throw for
   valid mapped objects, so the vector's normal path is an ordinary C loop.
   Generic C++ exception unwinding is not implemented by this specialization. */
void *restore_actor_ring_construct(void *object)
{
    volatile uint32_t *self=object;
    self[2]=0;
    self[1]=0;
    self[0]=0;
    return object;
}
uint32_t restore_actor_ring_destroy(void *object)
{return restore_actor_ring_clear(object);}

/* These two member entries and both CRT thunks were absent from units.json.
   The constructor's EAX is self; the destructor tail-jumps to clear (EAX0). */
RESTORE_RING_ENTRY0(FUN_004f6500,restore_actor_ring_construct)
RESTORE_RING_ENTRY0(FUN_004f6510,restore_actor_ring_destroy)

void FUN_004f64e0(void)
{
    uint32_t count=50u;
    while(count){
        --count;
        (void)restore_actor_ring_destroy((void *)(uintptr_t)(0xeaaf68u+count*224u));
    }
}
int FUN_004f64a0(void)
{
    FUN_004f64b0();
    return FUN_004f64d0();
}
