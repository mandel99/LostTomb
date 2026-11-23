#ifndef PHARAOH_RENDERING_UPPER_SPRITE_DRAW_H
#define PHARAOH_RENDERING_UPPER_SPRITE_DRAW_H

#include "pharaoh/assets/sprite_libraries.h"
#include "pharaoh/rendering/primitives.h"

#include <stdint.h>

typedef struct PharaohUpperSpriteDrawContext {
    const PharaohSpriteImageRecord *images;
    const uint8_t *const *image_pixels;
    int32_t image_count;
    PharaohSpriteClipState *clip;
    uint8_t *surface_pixels;
    int32_t surface_pitch_bytes;
    uint16_t tint_mask;
    const uint8_t **source_cursor; /* shared native D3B5B8 output */
} PharaohUpperSpriteDrawContext;

int32_t pharaoh_image_draw_isometric_top(
    const PharaohUpperSpriteDrawContext *context,
    int32_t image_id, int32_t x, int32_t y);

#endif
