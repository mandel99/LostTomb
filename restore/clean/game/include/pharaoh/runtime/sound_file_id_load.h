#ifndef PHARAOH_RUNTIME_SOUND_FILE_ID_LOAD_H
#define PHARAOH_RUNTIME_SOUND_FILE_ID_LOAD_H

#include <stdint.h>

#include "pharaoh/runtime/sound_file_table.h"

typedef struct PharaohSoundFileIdFileOperations {
    void *user_data;
    void *(*open_file)(void *user_data, const char *path, const char *mode,
        uint32_t open_flags);
    void (*close_file)(void *user_data, void *file);
} PharaohSoundFileIdFileOperations;

typedef struct PharaohSoundFileIdLoadContext {
    PharaohSoundFileTable *table;
    const PharaohSoundFileMemoryOperations *memory;
    const PharaohSoundFileIdFileOperations *file_operations;
    uint8_t fallback_drive_letter;
} PharaohSoundFileIdLoadContext;

/* Original address 0x00539C60. */
uint32_t pharaoh_sound_file_id_load(
    const PharaohSoundFileIdLoadContext *context, const char *path);

#endif
