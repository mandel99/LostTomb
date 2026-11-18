#include "pharaoh/runtime/file_read_part.h"

#include <string.h>

#include "pharaoh/runtime/file_path_helpers.h"

enum {
    NATIVE_FILE_PATH_ROOT_PREFIX_BYTES = 3,
    NATIVE_FILE_READ_OFFSET_ORIGIN = 0,
    NATIVE_FILE_SECONDARY_PATH_BYTES = 65,
    NATIVE_FILE_SECONDARY_PREFIX_BYTES = 4
};

static const char native_file_secondary_prefix[] = "555\\";

static int32_t open_with_media_drive_retry(
    const PharaohFileReadPartContext *context, const char *path)
{
    const PharaohFileSizeOperations *const operations =
        context->files->operations;
    const uint32_t flags = PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000;

    int32_t file_handle = operations->open_file(context->files->user_data,
        path, flags);
    if (file_handle != -1) {
        return file_handle;
    }

    char *const media_path = context->media_prefixed_path;
    media_path[0] = context->media_root->media_drive_letter;
    media_path[NATIVE_FILE_PATH_ROOT_PREFIX_BYTES] = '\0';
    const size_t path_bytes = strlen(path) + 1;
    memcpy(media_path + NATIVE_FILE_PATH_ROOT_PREFIX_BYTES, path, path_bytes);

    file_handle = operations->open_file(context->files->user_data,
        media_path, flags);
    int32_t retry_result = 1;
    while (file_handle == -1 && retry_result == 1) {
        retry_result = pharaoh_file_load_retry_prompt(
            context->retry_text_selection);
        file_handle = operations->open_file(context->files->user_data,
            media_path, flags);
    }
    return file_handle;
}

static void read_from_open_file(
    const PharaohFileReadPartContext *context, int32_t file_handle,
    void *destination, uint32_t byte_count, int32_t offset,
    int32_t *read_result)
{
    if (context->operations->seek_file(context->user_data, file_handle,
            offset, NATIVE_FILE_READ_OFFSET_ORIGIN) != -1) {
        *read_result = context->operations->read_file(context->user_data,
            file_handle, destination, byte_count);
    }
}

static void read_and_close_if_open(
    const PharaohFileReadPartContext *context, int32_t file_handle,
    void *destination, uint32_t byte_count, int32_t offset,
    int32_t *read_result)
{
    if (file_handle == -1) {
        return;
    }

    read_from_open_file(context, file_handle, destination, byte_count,
        offset, read_result);
    (void)context->files->operations->close_file(
        context->files->user_data, file_handle);
}

int32_t pharaoh_file_read_part(const PharaohFileReadPartContext *context,
                               const char *path, void *destination,
                               int32_t byte_count, int32_t offset)
{
    int32_t read_result = 0;
    int32_t file_handle = open_with_media_drive_retry(context, path);
    read_and_close_if_open(context, file_handle, destination,
        (uint32_t)byte_count, offset, &read_result);
    if (read_result > 0) {
        return read_result;
    }

    char secondary_path[NATIVE_FILE_SECONDARY_PATH_BYTES];
    memset(secondary_path, 0, sizeof secondary_path);
    pharaoh_file_path_capture_extension(path,
        context->media_root->path_state->extension);
    pharaoh_ascii_uppercase_in_place(
        (uint8_t *)context->media_root->path_state->extension);

    char *filename_destination = secondary_path;
    if (strcmp(context->media_root->path_state->extension, "555") == 0) {
        memcpy(secondary_path, native_file_secondary_prefix,
            sizeof native_file_secondary_prefix);
        filename_destination += NATIVE_FILE_SECONDARY_PREFIX_BYTES;
    }
    const size_t path_bytes = strlen(path) + 1;
    memcpy(filename_destination, path, path_bytes);

    file_handle = context->files->operations->open_file(
        context->files->user_data, secondary_path,
        PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000);
    read_and_close_if_open(context, file_handle, destination,
        (uint32_t)byte_count, offset, &read_result);
    if (read_result > 0) {
        return read_result;
    }

    (void)pharaoh_file_change_to_media_root(context->media_root,
        secondary_path);
    file_handle = context->files->operations->open_file(
        context->files->user_data, secondary_path,
        PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000);
    read_and_close_if_open(context, file_handle, destination,
        (uint32_t)byte_count, offset, &read_result);
    pharaoh_file_directory_reset(context->directory_reset);
    return read_result;
}
