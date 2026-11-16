#include "pharaoh/runtime/file_loading.h"
#include "pharaoh/runtime/game_data_loading.h"
#include "pharaoh/runtime/settings_ini.h"
#include "pharaoh/runtime/text_loading.h"

#include <string.h>

enum {
    PHARAOH_GAME_DATA_PATH_PREFIX_BYTES = 3,
    PHARAOH_SEEK_FROM_BEGINNING = 0
};

static PharaohReadOnlyFile *open_game_data_with_retry(
    const PharaohGameDataLoadContext *context, const char *filename)
{
    const PharaohFileReadOperations *operations = context->files->operations;
    PharaohReadOnlyFile *file = operations->open(context->files->user_data,
        filename);
    if (file != NULL) {
        return file;
    }

    char *fallback_path = context->state->fallback_path;
    fallback_path[0] = (char)context->settings->cd_drive_letter;
    fallback_path[PHARAOH_GAME_DATA_PATH_PREFIX_BYTES] = '\0';
    const size_t filename_bytes = strlen(filename) + 1;
    memcpy(fallback_path + PHARAOH_GAME_DATA_PATH_PREFIX_BYTES,
        filename, filename_bytes);

    file = operations->open(context->files->user_data, fallback_path);
    int32_t retry_result = 1;
    while (file == NULL) {
        if (retry_result != 1) {
            return NULL;
        }
        retry_result = pharaoh_file_load_retry_prompt(context->text_selection);
        file = operations->open(context->files->user_data, fallback_path);
    }
    return file;
}

int32_t pharaoh_game_data_file_size_from_offset(
    const PharaohGameDataLoadContext *context, const char *filename,
    int32_t offset)
{
    const PharaohFileReadOperations *operations = context->files->operations;
    PharaohReadOnlyFile *file = open_game_data_with_retry(context, filename);
    if (file == NULL) {
        return 0;
    }

    const int32_t file_size = (int32_t)operations->size(
        context->files->user_data, file);
    operations->close(context->files->user_data, file);
    return file_size - offset;
}

int32_t pharaoh_load_game_data(const PharaohGameDataLoadContext *context,
                               const char *filename, void *destination,
                               int32_t skip_bytes)
{
    const PharaohFileReadOperations *operations = context->files->operations;
    PharaohReadOnlyFile *file = open_game_data_with_retry(context, filename);
    if (file == NULL) {
        return 0;
    }

    const uint32_t file_size = (uint32_t)operations->size(
        context->files->user_data, file);
    (void)operations->seek(context->files->user_data, file,
        (int32_t)skip_bytes, PHARAOH_SEEK_FROM_BEGINNING);
    const uint32_t bytes_to_read = file_size - (uint32_t)skip_bytes;
    (void)operations->read(context->files->user_data, destination, 1,
        bytes_to_read, file);
    operations->close(context->files->user_data, file);
    return 1;
}
