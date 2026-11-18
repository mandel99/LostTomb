#include "pharaoh/runtime/sound_file_id_load.h"

#include <string.h>

enum {
    PHARAOH_SOUND_FILE_FALLBACK_PATH_CAPACITY = 260,
    PHARAOH_SOUND_FILE_OPEN_FLAGS = 0x40
};

static const char sound_file_binary_read_mode[] = "rb";

uint32_t pharaoh_sound_file_id_load(
    const PharaohSoundFileIdLoadContext *context, const char *path)
{
    char fallback_path[PHARAOH_SOUND_FILE_FALLBACK_PATH_CAPACITY];
    const char *registered_path = path;
    void *file = context->file_operations->open_file(
        context->file_operations->user_data, path,
        sound_file_binary_read_mode, PHARAOH_SOUND_FILE_OPEN_FLAGS);
    int32_t copy_path = 0;
    int32_t file_id;

    if (file == NULL) {
        fallback_path[0] = (char)context->fallback_drive_letter;
        fallback_path[1] = ':';
        fallback_path[2] = '\\';
        memcpy(&fallback_path[3], path, strlen(path) + 1u);

        file = context->file_operations->open_file(
            context->file_operations->user_data, fallback_path,
            sound_file_binary_read_mode, PHARAOH_SOUND_FILE_OPEN_FLAGS);
        if (file == NULL) {
            return UINT32_MAX;
        }

        registered_path = fallback_path;
        copy_path = 1;
    }

    file_id = pharaoh_sound_file_table_add(context->table,
        registered_path, copy_path, context->memory);
    context->file_operations->close_file(
        context->file_operations->user_data, file);
    return (uint32_t)file_id;
}
