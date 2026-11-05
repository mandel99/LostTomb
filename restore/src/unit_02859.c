#include "crt_exception_dispatch.h"

void restore_crt_search_handlers(uint32_t exception, uint32_t frame,
    uint32_t context, uint32_t reserved, uint32_t descriptor,
    uint8_t must_catch, uint32_t depth, uint32_t registration)
{
    int32_t state = restore_crt_eh_read_s32(frame + 8u);

    if (state < -1 || state >= restore_crt_eh_read_s32(descriptor + 4u))
        FUN_00563844();

    if (restore_crt_eh_read32(exception) == 0xe06d7363u) {
        if (restore_crt_eh_read32(exception + 16u) == 3u &&
            restore_crt_eh_read32(exception + 20u) == 0x19930520u &&
            restore_crt_eh_read32(exception + 28u) == 0u) {
            exception = restore_crt_eh_read32(0x01266418u);
            if (exception == 0u)
                return;
            context = restore_crt_eh_read32(0x0126641cu);
            if (FUN_0056876f((void *)(uintptr_t)exception, 1u) == 0u)
                FUN_00563844();
            if (restore_crt_eh_read32(exception) != 0xe06d7363u)
                goto non_cpp_exception;
            if (restore_crt_eh_read32(exception + 16u) == 3u &&
                restore_crt_eh_read32(exception + 20u) == 0x19930520u &&
                restore_crt_eh_read32(exception + 28u) == 0u)
                FUN_00563844();
        }

        if (restore_crt_eh_read32(exception) == 0xe06d7363u &&
            restore_crt_eh_read32(exception + 16u) == 3u &&
            restore_crt_eh_read32(exception + 20u) == 0x19930520u) {
            uint32_t first, limit;
            uint32_t block = (uint32_t)FUN_0056172c((int32_t)descriptor,
                (int32_t)depth, state, &first, &limit);

            for (; first < limit; ++first, block += 20u) {
                if (restore_crt_eh_read_s32(block) <= state &&
                    state <= restore_crt_eh_read_s32(block + 4u)) {
                    uint32_t handler = restore_crt_eh_read32(block + 16u);
                    int32_t handlers = restore_crt_eh_read_s32(block + 12u);

                    for (; handlers > 0; --handlers, handler += 16u) {
                        uint32_t throw_info = restore_crt_eh_read32(exception + 28u);
                        uint32_t conversions = restore_crt_eh_read32(throw_info + 12u);
                        int32_t remaining = restore_crt_eh_read_s32(conversions);
                        uint32_t conversion = conversions + 4u;

                        for (; remaining > 0; --remaining, conversion += 4u) {
                            if (FUN_00563b84((uint8_t *)(uintptr_t)handler,
                                (uint8_t *)(uintptr_t)restore_crt_eh_read32(conversion),
                                (uint32_t *)(uintptr_t)restore_crt_eh_read32(exception + 28u)) != 0u) {
                                /* Native pushes an eleventh local DWORD whose
                                   upper 24 bits are uninitialized. 563C95 never
                                   reads arg11; its largest argument offset is
                                   EBP+2C (arg10). Do not invent that input. */
                                FUN_00563c95(exception, (int32_t)frame, context,
                                    reserved, descriptor, (int32_t)handler,
                                    (int32_t)restore_crt_eh_read32(conversion),
                                    (uint32_t *)(uintptr_t)block, depth,
                                    (int32_t)registration);
                                goto next_try_block;
                            }
                        }
                    }
                }
next_try_block:;
            }
            if (must_catch != 0u)
                FUN_0056400a((int32_t)exception, 1u);
            return;
        }
    }

non_cpp_exception:
    if (must_catch == 0u) {
        FUN_00563adc(exception, frame, context, reserved, descriptor,
            state, depth, registration);
        return;
    }
    restore_crt_terminate();
}
