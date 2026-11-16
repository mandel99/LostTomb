#ifndef PHARAOH_ASSETS_SPRITE_LIBRARIES_H
#define PHARAOH_ASSETS_SPRITE_LIBRARIES_H

#include "pharaoh/assets/sprite_pixel_loading.h"

#include <stdint.h>

enum {
    PHARAOH_SPRITE_LIBRARY_NAME_BYTES = 260,
    PHARAOH_SPRITE_LIBRARY_COUNT = 30,
    PHARAOH_SPRITE_LIBRARY_BITMAP_COUNT = 200,
    PHARAOH_SPRITE_LIBRARY_BITMAP_NAME_BYTES = 200,
    PHARAOH_SPRITE_IMAGE_RECORD_BYTES = 64
};

typedef struct PharaohSpriteLibrary {
    char name[PHARAOH_SPRITE_LIBRARY_NAME_BYTES];
    uint8_t *pixels;
    uint32_t pixel_bytes;
    uint32_t image_count;
    uint32_t tag_count;
    uint32_t first_image;
    uint32_t first_tag;
    char bitmap_names[PHARAOH_SPRITE_LIBRARY_BITMAP_COUNT]
        [PHARAOH_SPRITE_LIBRARY_BITMAP_NAME_BYTES];
} PharaohSpriteLibrary;

typedef struct PharaohSpriteImageRecord {
    uint32_t pixel_offset;
    uint32_t pixel_bytes;
    uint32_t isometric_top_bytes;
    uint32_t library_index;
    uint8_t reserved_10[12];
    uint16_t tag_id;
    uint8_t reserved_1e[20];
    uint8_t draw_type;
    uint8_t is_compressed;
    uint8_t is_external;
    uint8_t has_isometric_top;
    uint8_t reserved_36[10];
} PharaohSpriteImageRecord;

typedef struct PharaohSpriteTagRecord {
    uint32_t library_index;
    uint32_t image_index;
} PharaohSpriteTagRecord;

typedef struct PharaohSpriteLibraryReadContext {
    const PharaohGameDataLoadContext *game_data;
    const PharaohSpritePixelLoadContext *pixels;
    uint32_t *pixel_cursor;
} PharaohSpriteLibraryReadContext;

_Static_assert(sizeof(PharaohSpriteImageRecord) ==
               PHARAOH_SPRITE_IMAGE_RECORD_BYTES,
               "sprite image records retain the native 64-byte format");

/* Typed state used by the reset routine; it is not the packed native manager. */
typedef struct PharaohSpriteLibraryResetState {
    PharaohSpriteLibrary items[PHARAOH_SPRITE_LIBRARY_COUNT];
    uint32_t tag_limits[PHARAOH_SPRITE_LIBRARY_COUNT];
    uint8_t *scratch_pixel_buffer;
    uint8_t *alternate_pixel_buffer;
    uint32_t next_tag;
    uint32_t next_image;
    uint32_t pixel_bytes;
    uint32_t loaded_count;
} PharaohSpriteLibraryResetState;

typedef void (*PharaohSpriteTagSyncWarningOperation)(
    void *user_data, const char *library_name, uint32_t actual_tags,
    uint32_t expected_tags);

typedef struct PharaohSpriteLibraryLoadContext {
    PharaohSpriteLibraryResetState *libraries;
    const PharaohSpriteLibraryReadContext *reader;
    PharaohSpriteImageRecord *images;
    PharaohSpriteTagRecord *tags;
    PharaohSpriteTagSyncWarningOperation show_tag_sync_warning;
    void *warning_user_data;
} PharaohSpriteLibraryLoadContext;

void pharaoh_sprite_libraries_reset(
    PharaohSpriteLibraryResetState *libraries,
    const PharaohSpritePixelLoadContext *pixel_context);
uint8_t pharaoh_sprite_libraries_load_one(
    const PharaohSpriteLibraryLoadContext *context,
    const char *const names[PHARAOH_SPRITE_LIBRARY_COUNT]);
uint8_t pharaoh_sprite_library_load_file(
    const PharaohSpriteLibraryLoadContext *context,
    const char *name, int32_t library_index);
uint8_t pharaoh_sprite_library_load_named(
    const PharaohSpriteLibraryLoadContext *context, const char *name);
void pharaoh_sprite_libraries_load_all(
    const PharaohSpriteLibraryLoadContext *context,
    const char *const names[PHARAOH_SPRITE_LIBRARY_COUNT]);
uint8_t pharaoh_sprite_library_read(
    const PharaohSpriteLibraryReadContext *context,
    PharaohSpriteLibrary *library, const char *name, int32_t library_index,
    PharaohSpriteImageRecord *images, uint32_t first_image,
    PharaohSpriteTagRecord *tags, uint32_t first_tag);

#endif
