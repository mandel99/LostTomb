#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <errno.h>
#include <stdint.h>
#include <string.h>

#include "pharaoh/runtime/file_directory_reset.h"
#include "pharaoh/runtime/current_directory.h"

static const char directory_reset_error_message[] =
    "ERR:CHD  not reserted is now";

enum {
    NATIVE_DIRECTORY_QUERY_BYTES = 0x104,
    NATIVE_DIRECTORY_DETAIL_BYTES = 100,
    NATIVE_DIRECTORY_QUERY_LIMIT = 0x105
};

static void capture_directory_detail(PharaohCrtErrorState *crt_error_state,
                                     char *destination)
{
    char current_directory[NATIVE_DIRECTORY_QUERY_BYTES];
    const DWORD path_length = GetCurrentDirectoryA(
        NATIVE_DIRECTORY_QUERY_BYTES, current_directory);
    if (path_length == 0 ||
        path_length + 1 >= NATIVE_DIRECTORY_QUERY_LIMIT) {
        return;
    }

    const uint32_t required_bytes = path_length + 1;
    if (required_bytes > NATIVE_DIRECTORY_DETAIL_BYTES) {
        crt_error_state->errno_value = EINVAL;
        return;
    }

    memcpy(destination, current_directory, required_bytes);
}

void pharaoh_file_directory_reset(
    const PharaohFileDirectoryResetContext *context)
{
    if (pharaoh_set_current_directory(context->crt_error_state,
            context->startup_directory) != 0) {
        capture_directory_detail(context->crt_error_state,
            context->current_directory_detail);
        pharaoh_status_log_format(directory_reset_error_message,
            context->current_directory_detail, 0,
            context->status_log_state, context->status_log_operations);
    }
}
