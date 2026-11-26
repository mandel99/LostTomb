#include "pharaoh/runtime/crt_lowio.h"

#include <stdint.h>
#include <windows.h>

uint32_t pharaoh_crt_get_os_handle(
    PharaohCrtLowioState *state, uint32_t descriptor)
{
    if (descriptor < state->capacity) {
        const int32_t block = (int32_t)descriptor >> 5;
        PharaohCrtLowioRecord *record =
            state->blocks[block] + (descriptor & 31u);

        if ((record->flags & 1u) != 0u) return record->handle;
    }

    *state->dos_error = 0;
    *state->error_number = 9;
    return UINT32_MAX;
}

/* Complete typed translation of Pharaoh.exe 0x005668D5. */
uint32_t pharaoh_crt_set_os_handle(PharaohCrtLowioState *state,
    uint32_t descriptor, uint32_t handle_bits)
{
    if (descriptor < state->capacity) {
        const uint32_t block = descriptor >> 5;
        const uint32_t slot = descriptor & 31u;
        PharaohCrtLowioRecord *record = state->blocks[block] + slot;

        if (record->handle == UINT32_MAX) {
            if (state->standard_handle_mode == 1u && descriptor <= 2u) {
                (void)SetStdHandle(UINT32_C(0xfffffff6) - descriptor,
                    (HANDLE)(uintptr_t)handle_bits);
            }
            record = state->blocks[block] + slot;
            record->handle = handle_bits;
            return 0;
        }
    }

    *state->dos_error = 0;
    *state->error_number = 9;
    return UINT32_MAX;
}

/* Complete typed translation of Pharaoh.exe 0x0056694C. */
uint32_t pharaoh_crt_free_os_handle(PharaohCrtLowioState *state,
    uint32_t descriptor)
{
    if (descriptor < state->capacity) {
        const uint32_t block_index = descriptor >> 5;
        const uint32_t slot = descriptor & 31u;
        PharaohCrtLowioRecord *volatile *const block =
            state->blocks + block_index;
        volatile PharaohCrtLowioRecord *record = *block + slot;

        if ((record->flags & 1u) != 0u && record->handle != UINT32_MAX) {
            if (state->standard_handle_mode == 1u && descriptor <= 2u) {
                (void)SetStdHandle(UINT32_C(0xfffffff6) - descriptor,
                    (HANDLE)0);
            }
            record = *block + slot;
            record->handle = UINT32_MAX;
            return 0;
        }
    }

    *state->dos_error = 0;
    *state->error_number = 9;
    return UINT32_MAX;
}

/* Complete zero-callee C translation of Pharaoh.exe 0x0056B22E. */
uint32_t pharaoh_crt_stream_descriptor(const PharaohCrtFileStream *stream)
{
    return stream->descriptor;
}
