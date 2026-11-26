#include "pharaoh/runtime/crt_lowio.h"
#include "pharaoh/runtime/crt_error_translation.h"

#include <stdint.h>
#include <windows.h>

/* Complete typed C translation of Pharaoh.exe 0x005662E3. */
uint32_t pharaoh_crt_seek_descriptor(PharaohCrtLowioState *state,
    uint32_t descriptor, int32_t offset, uint32_t origin)
{
    if (descriptor < state->capacity) {
        PharaohCrtLowioRecord *volatile *const block =
            state->blocks + (descriptor >> 5);
        volatile PharaohCrtLowioRecord *record =
            *block + (descriptor & 31u);

        if ((record->flags & 1u) != 0u) {
            const uint32_t handle = pharaoh_crt_get_os_handle(state,
                descriptor);
            if (handle == UINT32_MAX) {
                *state->error_number = 9;
                return UINT32_MAX;
            }

            const DWORD position = SetFilePointer(
                (HANDLE)(uintptr_t)handle, (LONG)offset, NULL, origin);
            const DWORD win32_error = position == INVALID_SET_FILE_POINTER
                ? GetLastError() : 0u;
            if (win32_error != 0u) {
                PharaohCrtErrorState error_state = {
                    *state->dos_error, *state->error_number
                };
                pharaoh_crt_error_from_win32(&error_state, win32_error);
                *state->dos_error = error_state.dos_error;
                *state->error_number = error_state.errno_value;
                return UINT32_MAX;
            }

            record = *block + (descriptor & 31u);
            record->flags &= 0xfdu;
            return position;
        }
    }

    *state->dos_error = 0;
    *state->error_number = 9;
    return UINT32_MAX;
}

int32_t pharaoh_crt_close_descriptor(PharaohCrtLowioState *state,
    uint32_t descriptor)
{
    uint32_t win32_error = 0;

    if (descriptor >= state->capacity) {
        *state->dos_error = 0;
        *state->error_number = 9;
        return -1;
    }

    PharaohCrtLowioRecord *volatile *const block =
        state->blocks + (descriptor >> 5);
    volatile PharaohCrtLowioRecord *record = *block + (descriptor & 31u);
    if ((record->flags & 1u) == 0u) {
        *state->dos_error = 0;
        *state->error_number = 9;
        return -1;
    }

    if (pharaoh_crt_get_os_handle(state, descriptor) != UINT32_MAX) {
        uint32_t shared = 0;
        if (descriptor == 1u || descriptor == 2u) {
            const uint32_t stderr_handle = pharaoh_crt_get_os_handle(state, 2u);
            const uint32_t stdout_handle = pharaoh_crt_get_os_handle(state, 1u);
            shared = stderr_handle == stdout_handle;
        }
        if (shared == 0u) {
            const uint32_t handle = pharaoh_crt_get_os_handle(state, descriptor);
            if (!CloseHandle((HANDLE)(uintptr_t)handle))
                win32_error = GetLastError();
        }
    }

    (void)pharaoh_crt_free_os_handle(state, descriptor);
    record = *block + (descriptor & 31u);
    record->flags = 0;
    if (win32_error != 0u) {
        PharaohCrtErrorState error_state = {
            *state->dos_error, *state->error_number
        };
        pharaoh_crt_error_from_win32(&error_state, win32_error);
        *state->dos_error = error_state.dos_error;
        *state->error_number = error_state.errno_value;
        return -1;
    }
    return 0;
}

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
