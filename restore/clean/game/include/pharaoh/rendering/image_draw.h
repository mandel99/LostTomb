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

struct PharaohBitmapFont;
typedef struct PharaohImageGlyphDrawBinding {
    PharaohImageDrawContext *drawing;
    uint16_t *color; /* actual native D3B51C, shared with text drawing */
} PharaohImageGlyphDrawBinding;
/* Concrete0053E590 endpoint for004CCA60; uses the already locked target. */
void pharaoh_image_bind_bitmap_font(struct PharaohBitmapFont *,PharaohImageGlyphDrawBinding *);

/* Complete native0053E590 behavior with explicit assets, pixels and IO state.
 * Metadata links, image buffers and bitmap indices must be valid native data. */
int32_t pharaoh_image_draw(PharaohImageDrawContext *context,
    int32_t image,int32_t x,int32_t y);
/* Native0053F4F0; image_pixels must contain the actual resident pointers. */
int32_t pharaoh_image_draw_masked(PharaohImageDrawContext *context,
    int32_t image, int32_t x, int32_t y, uint32_t mask);
struct PharaohCityGroundOperations;
void pharaoh_city_ground_bind_image_draw(struct PharaohCityGroundOperations *operations,
    PharaohImageDrawContext *context);
void pharaoh_city_ground_bind_sprite_draw(struct PharaohCityGroundOperations *operations,
    PharaohImageDrawContext *context);
#endif
