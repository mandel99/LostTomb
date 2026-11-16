#ifndef PHARAOH_RUNTIME_GAME_DATA_LOADING_H
#define PHARAOH_RUNTIME_GAME_DATA_LOADING_H

#include "pharaoh/runtime/file_loading.h"
#include "pharaoh/runtime/settings_ini.h"
#include "pharaoh/runtime/text_loading.h"

typedef struct PharaohGameDataLoadContext {
    const PharaohFileReadContext *files;
    const PharaohSettingsFile *settings;
    PharaohTextSelection *text_selection;
    struct PharaohGameDataLoadState *state;
} PharaohGameDataLoadContext;

enum {
    PHARAOH_GAME_DATA_PATH_BYTES = 260
};

typedef struct PharaohGameDataLoadState {
    char fallback_path[PHARAOH_GAME_DATA_PATH_BYTES];
} PharaohGameDataLoadState;

#define PHARAOH_GAME_DATA_LOAD_STATE_INITIALIZER { "C:\\" }

int32_t pharaoh_game_data_file_size_from_offset(
    const PharaohGameDataLoadContext *context, const char *filename,
    int32_t offset);
int32_t pharaoh_load_game_data(const PharaohGameDataLoadContext *context,
                               const char *filename, void *destination,
                               int32_t skip_bytes);

#endif
