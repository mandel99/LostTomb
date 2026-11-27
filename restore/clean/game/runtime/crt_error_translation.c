#include "pharaoh/runtime/crt_error_translation.h"

#include <errno.h>
#include <stddef.h>

typedef struct PharaohWin32ErrnoMapping {
    uint32_t win32_error;
    int32_t errno_value;
} PharaohWin32ErrnoMapping;

static const PharaohWin32ErrnoMapping win32_errno_mappings[] = {
    { 1, 22 }, { 2, 2 }, { 3, 2 }, { 4, 24 }, { 5, 13 },
    { 6, 9 }, { 7, 12 }, { 8, 12 }, { 9, 12 }, { 10, 7 },
    { 11, 8 }, { 12, 22 }, { 13, 22 }, { 15, 2 }, { 16, 13 },
    { 17, 18 }, { 18, 2 }, { 33, 13 }, { 53, 2 }, { 65, 13 },
    { 67, 2 }, { 80, 17 }, { 82, 13 }, { 83, 13 }, { 87, 22 },
    { 89, 11 }, { 108, 13 }, { 109, 32 }, { 112, 28 }, { 114, 9 },
    { 6, 22 }, { 128, 10 }, { 129, 10 }, { 130, 9 }, { 131, 22 },
    { 132, 13 }, { 145, 41 }, { 158, 13 }, { 161, 2 }, { 164, 11 },
    { 167, 13 }, { 183, 17 }, { 206, 2 }, { 215, 11 }, { 1816, 12 }
};

void pharaoh_crt_error_from_win32(PharaohCrtErrorState *state,
                                  uint32_t win32_error)
{
    state->dos_error = win32_error;

    for (size_t index = 0;
         index < sizeof win32_errno_mappings / sizeof win32_errno_mappings[0];
         ++index) {
        if (win32_error == win32_errno_mappings[index].win32_error) {
            state->errno_value = win32_errno_mappings[index].errno_value;
            return;
        }
    }

    if (win32_error > 0x12 && win32_error < 0x25) {
        state->errno_value = EACCES;
    } else if (win32_error >= 0xbc && win32_error <= 0xca) {
        state->errno_value = ENOEXEC;
    } else {
        state->errno_value = EINVAL;
    }
}

uint32_t pharaoh_crt_math_hook_disabled(void)
{
    return 0;
}

void pharaoh_crt_set_math_error(int32_t argument, int32_t *error_code)
{
    if (argument == 1)
        *error_code = 0x21;
    else if (argument > 1 && argument < 4)
        *error_code = 0x22;
}
