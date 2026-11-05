#include "crt_exception_dispatch.h"

/* The separately linked crt_exception_handlers.asm registers FUN_005615de
   in SafeSEH metadata. This function intentionally installs its native record. */
#pragma warning(push)
#pragma warning(disable:4733)
uint32_t restore_crt_invoke_registered(uint32_t frame, uint32_t descriptor,
    uint32_t handler, uint32_t depth, uint32_t code)
{
    volatile RestoreCrtRegistration5 record;
    uint32_t result;

    record.previous = 0;
    record.descriptor = descriptor;
    record.handler = (uint32_t)(uintptr_t)FUN_005615de;
    record.frame = frame;
    record.depth = depth + 1u;
    record.previous = __readfsdword(0);
    _ReadWriteBarrier();
    __writefsdword(0, (uint32_t)(uintptr_t)&record);
    _ReadWriteBarrier();

    result = restore_crt_invoke_frame(handler, frame, code);

    _ReadWriteBarrier();
    __writefsdword(0, record.previous);
    _ReadWriteBarrier();
    return result;
}
#pragma warning(pop)
