#ifndef PHARAOH_RENDERING_SPRITE_RAW_BLIT_H
#define PHARAOH_RENDERING_SPRITE_RAW_BLIT_H

#include <stdint.h>

typedef struct PharaohSpriteRawBlitTarget {
    uint8_t *surface_pixels;
    int32_t surface_pitch_bytes;
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    int32_t source_row_skip_bytes;
    int32_t destination_row_skip_bytes;
} PharaohSpriteRawBlitTarget;

/* Native 0x0040FFD0: full-width raw 565 copy with 0xF81F transparency. */
void pharaoh_sprite_raw_blit_keyed(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels);

/* Native 0x00410090: clipped raw copy with 0xF81F transparency. */
void pharaoh_sprite_raw_blit_keyed_clipped(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels);

/* Native 0x0040FB60: clipped raw copy without a transparency key. */
void pharaoh_sprite_raw_blit_clipped(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels);
void pharaoh_sprite_raw_blit_mirrored(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels);
void pharaoh_sprite_raw_blit_mirrored_clipped(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels);
void pharaoh_sprite_raw_mask(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels,
    uint16_t mask);
void pharaoh_sprite_raw_mask_clipped(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels,
    uint16_t mask);
void pharaoh_sprite_raw_blit_square16(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels);
void pharaoh_sprite_raw_blit_square24(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels);
void pharaoh_sprite_raw_blit_square32(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels);

const uint8_t *pharaoh_graphics_rle_skip_rows(
    const uint8_t *source, int32_t row_count, int32_t row_width);

void pharaoh_graphics_and_bytes(
    uint32_t *data, int32_t byte_count, uint8_t mask);
#endif
