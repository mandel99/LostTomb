#ifndef PHARAOH_RUNTIME_STARTUP_ASSET_LOADING_H
#define PHARAOH_RUNTIME_STARTUP_ASSET_LOADING_H

#include "pharaoh/assets/sprite_libraries.h"
#include "pharaoh/runtime/game_data_loading.h"

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_STARTUP_SPRITE_LIBRARY_COUNT = 30,
    PHARAOH_STARTUP_TEXT_GROUP = 11,
    PHARAOH_STARTUP_TEXT_FIRST_ENTRY = 7,
    PHARAOH_STARTUP_PIXEL_BUFFER_MINIMUM_BYTES = 1200000,
    PHARAOH_STARTUP_PIXEL_BUFFER_MINIMUM_TEST = 1200001
};

typedef struct PharaohStartupAssetState {
    uint8_t *map_info_file_buffer;
    uint8_t *screen_pixel_buffer;
    uint32_t loading_progress_percent;
    int32_t selected_campaign_id;
    uint32_t startup_asset_setup_flag;
} PharaohStartupAssetState;

typedef struct PharaohStartupAssetOperations {
    void *user_data;
    void (*create_directory)(void *user_data, const char *path);
    void *(*allocate)(void *user_data, size_t byte_count);
    void (*clear_loading_surface)(void *user_data);
    void (*draw_sprite)(void *user_data, uint32_t image_id, int32_t x,
                        int32_t y);
    void (*draw_status_text)(void *user_data, const char *text);
    void (*process_loading_updates)(void *user_data, int32_t force_update);
    void (*write_startup_status)(void *user_data, const char *text);
} PharaohStartupAssetOperations;

typedef struct PharaohStartupAssetLoadContext {
    const PharaohGameDataLoadContext *game_data;
    const PharaohSpriteLibraryLoadContext *sprite_libraries;
    PharaohTextSelection *text_selection;
    PharaohStartupAssetState *state;
    const PharaohStartupAssetOperations *operations;
    uint32_t screen_width;
    uint32_t screen_height;
    int32_t loading_image_x;
    int32_t loading_image_y;
    uint32_t loading_stage_image_base;
    uint32_t loading_background_image;
} PharaohStartupAssetLoadContext;

void pharaoh_loading_screen_draw_stage(
    const PharaohStartupAssetLoadContext *context, int32_t stage_index);
void pharaoh_loading_screen_draw_background(
    const PharaohStartupAssetLoadContext *context);
int32_t pharaoh_startup_asset_setup(
    const PharaohStartupAssetLoadContext *context);

#endif
