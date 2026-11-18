#include "pharaoh/runtime/file_media_root.h"

#include <string.h>

#include "pharaoh/runtime/current_directory.h"
#include "pharaoh/runtime/file_path_helpers.h"

enum {
    NATIVE_MEDIA_ROOT_BUFFER_BYTES = 40,
    NATIVE_MEDIA_ROOT_TEMPLATE_BYTES = 10
};

static const char native_media_root_template[NATIVE_MEDIA_ROOT_TEMPLATE_BYTES] = {
    'c', ':', '\\', '\0', '5', '5', '5', '\0', 'S', 'M'
};

static const char *const supported_media_extensions[] = {
    "WAV", "SMK", "555"
};

int32_t pharaoh_file_change_to_media_root(
    PharaohFileMediaRootContext *context, const char *path)
{
    pharaoh_file_path_capture_extension(path, context->path_state->extension);
    pharaoh_ascii_uppercase_in_place(
        (uint8_t *)context->path_state->extension);

    int supported = 0;
    for (uint32_t index = 0;
         index < sizeof supported_media_extensions /
             sizeof supported_media_extensions[0]; ++index) {
        if (strcmp(context->path_state->extension,
                supported_media_extensions[index]) == 0) {
            supported = 1;
            break;
        }
    }
    if (supported == 0) {
        return 0;
    }

    memset(context->path_state->media_fallback_directory, 0,
        NATIVE_MEDIA_ROOT_BUFFER_BYTES);
    memcpy(context->path_state->media_fallback_directory,
        native_media_root_template,
        NATIVE_MEDIA_ROOT_TEMPLATE_BYTES);
    context->path_state->media_fallback_directory[0] =
        context->media_drive_letter;

    return pharaoh_set_current_directory(context->crt_error_state,
        context->path_state->media_fallback_directory) == 0;
}
