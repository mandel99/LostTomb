#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <ctype.h>
#include <stdint.h>

#include "pharaoh/runtime/current_directory.h"

enum { NATIVE_CURRENT_DIRECTORY_BUFFER_BYTES = 0x105 };

int32_t pharaoh_set_current_directory(PharaohCrtErrorState *crt_error_state,
                                      const char *directory)
{
    char current_directory[NATIVE_CURRENT_DIRECTORY_BUFFER_BYTES];

    if (SetCurrentDirectoryA(directory) == 0) {
        goto record_error;
    }
    if (GetCurrentDirectoryA(NATIVE_CURRENT_DIRECTORY_BUFFER_BYTES,
            current_directory) == 0) {
        goto record_error;
    }

    if (((current_directory[0] != '\\') && (current_directory[0] != '/')) ||
        current_directory[0] != current_directory[1]) {
        char drive_environment_name[3];
        drive_environment_name[0] = (char)toupper(
            (unsigned char)current_directory[0]);
        drive_environment_name[1] = ':';
        drive_environment_name[2] = '\0';
        if (SetEnvironmentVariableA(drive_environment_name,
                current_directory) == 0) {
            goto record_error;
        }
    }

    return 0;

record_error:
    pharaoh_crt_error_from_win32(crt_error_state, GetLastError());
    return -1;
}
