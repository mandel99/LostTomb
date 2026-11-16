#ifndef PHARAOH_ASSETS_SPRITE_PIXEL_LOADING_H
#define PHARAOH_ASSETS_SPRITE_PIXEL_LOADING_H

#include "pharaoh/runtime/game_data_loading.h"

#include <stdint.h>

typedef struct PharaohSpritePixelMemoryOperations {
    void *(*allocate)(void *user_data, uint32_t byte_count);
    void (*release)(void *user_data, void *allocation);
} PharaohSpritePixelMemoryOperations;

typedef struct PharaohSpritePixelLoadContext {
    const PharaohGameDataLoadContext *game_data;
    const PharaohSpritePixelMemoryOperations *memory_operations;
    void *memory_user_data;
    void *shared_pixel_buffer;
    int32_t pixel_mode;
} PharaohSpritePixelLoadContext;

void pharaoh_sprite_release_pixels(const PharaohSpritePixelLoadContext *context,
    uint8_t **pixels, int32_t preserve_shared_buffer);
uint32_t pharaoh_sprite_load_pixels(const PharaohSpritePixelLoadContext *context,
    const char *name, uint8_t **pixels, uint8_t convert,
    uint32_t byte_count, uint8_t compressed, int32_t destination_offset,
    uint32_t file_offset);

#endif
