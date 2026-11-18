#include "pharaoh/runtime/file_exists_media_fallback.h"

int32_t pharaoh_file_exists_with_media_fallback(
    const PharaohFileExistsMediaFallbackContext *context,
    const char *filename)
{
    const PharaohFileSizeOperations *const operations =
        context->files->operations;
    int32_t file_handle = operations->open_file(context->files->user_data,
        filename, PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000);
    if (file_handle >= 0) {
        (void)operations->close_file(context->files->user_data, file_handle);
        return 1;
    }

    (void)pharaoh_file_change_to_media_root(context->media_root, filename);
    file_handle = operations->open_file(context->files->user_data, filename,
        PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000);
    if (file_handle >= 0) {
        (void)operations->close_file(context->files->user_data, file_handle);
    }

    pharaoh_file_directory_reset(context->directory_reset);
    return file_handle >= 0;
}
