#ifndef PHARAOH_RENDERING_SPRITE_RLE_BLIT_H
#define PHARAOH_RENDERING_SPRITE_RLE_BLIT_H

#include <stdint.h>

typedef struct PharaohSpriteRleBlitTarget {
    uint8_t *surface_pixels;
    int32_t surface_pitch_bytes;
    int32_t x;
    int32_t y;
    int32_t image_width;
    int32_t visible_height;
    int32_t source_rows_to_skip;
    int32_t destination_row_skip_bytes;
    int32_t left_clip_bytes;
    int32_t right_visible_width;
    uint16_t solid_color;
} PharaohSpriteRleBlitTarget;

void pharaoh_image_blit_compressed(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels);
void pharaoh_image_blit_compressed_left(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels);
void pharaoh_image_blit_compressed_right(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels);
void pharaoh_image_blit_compressed_mask(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels,
    uint16_t mask);
void pharaoh_image_blit_compressed_mask_left(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels,
    uint16_t mask);
void pharaoh_image_blit_compressed_mask_right(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels,
    uint16_t mask);
void pharaoh_image_blit_compressed_color(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels);
void pharaoh_image_blit_compressed_color_left(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels);
void pharaoh_image_blit_compressed_color_right(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels);
void pharaoh_image_blit_compressed_mirrored(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels);
void pharaoh_image_blit_compressed_mirrored_left(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels);
void pharaoh_image_blit_compressed_mirrored_right(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels);

#endif
