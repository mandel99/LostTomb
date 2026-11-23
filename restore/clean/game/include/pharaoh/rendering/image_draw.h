#ifndef PHARAOH_RENDERING_IMAGE_DRAW_H
#define PHARAOH_RENDERING_IMAGE_DRAW_H
#include "pharaoh/assets/sprite_libraries.h"
#include "pharaoh/assets/sprite_fetch_rows.h"
#include "pharaoh/rendering/primitives.h"

typedef union PharaohImageDrawCursor {
    const uint8_t *pixels;
    uint32_t offset;
} PharaohImageDrawCursor;

typedef struct PharaohImageDrawState {
    int32_t image, x, y;
    int32_t crop_x, crop_y, crop_width, crop_height;
    uint8_t draw_type;
    PharaohImageDrawCursor source;
} PharaohImageDrawState;

typedef struct PharaohImageDrawContext {
    const PharaohSpriteImageRecord *images;
    const uint8_t *const *image_pixels;
    const int32_t *image_count;
    const PharaohSpriteLibrary *libraries;
    PharaohImageDrawState *state;
    PharaohSpriteClipState *clip;
    uint8_t *surface_pixels;
    int32_t pitch_bytes;
    const uint16_t *tint, *solid_color;
    const int32_t *screen_width, *screen_height, *special_image, *pixel_mode;
    const uint8_t *window_id;
    uint32_t terrain_source_bias;
    const PharaohFileReadPartContext *files;
    const PharaohSpritePixelLoadContext *pixel_memory;
    uint8_t **external_pixels;
    char *bitmap_name_scratch; /* native persistent filename scratch */
} PharaohImageDrawContext;

/* Complete native0053E590 behavior with explicit assets, pixels and IO state.
 * Metadata links, image buffers and bitmap indices must be valid native data. */
int32_t pharaoh_image_draw(PharaohImageDrawContext *context,
    int32_t image,int32_t x,int32_t y);
struct PharaohCityGroundOperations;
void pharaoh_city_ground_bind_image_draw(struct PharaohCityGroundOperations *operations,
    PharaohImageDrawContext *context);
#endif
