#include "crt_exception_dispatch.h"

uint32_t restore_crt_registration_handler(uint32_t exception,
    uint32_t registration, uint32_t context)
{
    volatile const RestoreCrtRegistration5 *record =
        (volatile const RestoreCrtRegistration5 *)(uintptr_t)registration;

    /* The native handler clears DF before entering C exception dispatch. */
    __asm cld
    return FUN_0056389a((int32_t *)(uintptr_t)exception, record->frame,
        context, 0u, (int32_t *)(uintptr_t)record->descriptor,
        (int32_t)record->depth, registration, 0u);
}
