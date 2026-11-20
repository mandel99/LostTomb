#ifndef PHARAOH_SCENARIO_MAP_FILE_LOAD_H
#define PHARAOH_SCENARIO_MAP_FILE_LOAD_H

#include "pharaoh/runtime/file_size.h"

#include <stdint.h>

typedef struct PharaohMapFileLoadOperations {
    void *user_data;
    /* Native 0x004CE2D0; this restores the game's current directory. */
    void (*restore_application_directory)(void *user_data);
    /* Native 0x004BD330. */
    void (*load_scenario_settings)(void *user_data);
    /* Native 0x004D4AE0. */
    void (*parse_map_file)(void *user_data, int32_t file_handle);
    /* Native 0x004F4520, 0x0053D970, and 0x004BC720, in that order. */
    void (*initialize_map_assets)(void *user_data);
    /* Native rating helper receives 1 when the gate at 0x00605A3C is 1. */
    int32_t (*calculate_monument_rating)(void *user_data, int32_t factor);
} PharaohMapFileLoadOperations;

typedef struct PharaohMapFileLoadContext {
    const PharaohFileSizeContext *files;
    const PharaohMapFileLoadOperations *operations;
    int32_t *current_map_environment;
    const int16_t *selected_environment;
    const uint32_t *rating_update_enabled;
    int32_t *stored_monument_rating;
} PharaohMapFileLoadContext;

/* Native 0x004D5820 wrapper. Returns 1 when the path cannot be opened and 0
   after parsing; mode 0 also initializes map assets. */
int32_t pharaoh_map_file_load(
    const PharaohMapFileLoadContext *context, const char *path, int32_t mode);

#endif
