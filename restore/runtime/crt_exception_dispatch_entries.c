#include "crt_exception_dispatch.h"

uint32_t FUN_0056158a(uint32_t frame, uint32_t descriptor, uint32_t handler,
    uint32_t depth, uint32_t code)
{
    return restore_crt_invoke_registered(frame, descriptor, handler, depth, code);
}

uint32_t FUN_005615de(uint32_t exception, uint32_t registration, uint32_t context)
{
    return restore_crt_registration_handler(exception, registration, context);
}

void FUN_00563935(uint32_t exception, uint32_t frame, uint32_t context,
    uint32_t reserved, uint32_t descriptor, uint32_t must_catch,
    uint32_t depth, uint32_t registration)
{
    restore_crt_search_handlers(exception, frame, context, reserved,
        descriptor, (uint8_t)must_catch, depth, registration);
}
