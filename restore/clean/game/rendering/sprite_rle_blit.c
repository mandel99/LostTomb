#include "pharaoh/rendering/sprite_rle_blit.h"
#include "pharaoh/rendering/sprite_raw_blit.h"

#include <stddef.h>
#include <string.h>

enum {
    RLE_CLIP_NONE = 0,
    RLE_CLIP_LEFT = 1,
    RLE_CLIP_RIGHT = 2,
    RLE_COPY = 0,
    RLE_MASK = 1,
    RLE_SOLID = 2
};

static uint8_t *rle_destination_start(
    const PharaohSpriteRleBlitTarget *target)
{
    return target->surface_pixels +
        (ptrdiff_t)target->y * target->surface_pitch_bytes +
        (ptrdiff_t)target->x * (ptrdiff_t)sizeof(uint16_t);
}

static void rle_write_pixel(uint8_t *destination, const uint8_t *source,
    int32_t operation, uint16_t value)
{
    if (operation == RLE_COPY) {
        memcpy(destination, source, sizeof(uint16_t));
    } else if (operation == RLE_MASK) {
        uint16_t destination_pixel;
        memcpy(&destination_pixel, destination, sizeof(destination_pixel));
        destination_pixel &= value;
        memcpy(destination, &destination_pixel, sizeof(destination_pixel));
    } else {
        memcpy(destination, &value, sizeof(value));
    }
}

static void rle_blit(const PharaohSpriteRleBlitTarget *target,
    const uint8_t *pixels, int32_t clip, int32_t operation, uint16_t value)
{
    const uint8_t *source = pharaoh_graphics_rle_skip_rows(pixels,
        target->source_rows_to_skip, target->image_width);
    uint8_t *row_destination = rle_destination_start(target);
    int32_t rows = target->visible_height;

    do {
        int32_t remaining = target->image_width;
        int32_t cutoff = clip == RLE_CLIP_LEFT
            ? target->left_clip_bytes / 2
            : target->right_visible_width;
        uint8_t *destination = clip == RLE_CLIP_LEFT
            ? row_destination - (ptrdiff_t)cutoff * 2
            : row_destination;

        while (remaining > 0) {
            uint32_t run = *source++;
            if (run == 0xffu) {
                run = *source++;
                destination += run * 2u;
                if (clip != RLE_CLIP_NONE) cutoff -= (int32_t)run;
                remaining -= (int32_t)run;
                continue;
            }

            if (clip == RLE_CLIP_NONE && (run < 1u || run > 16u))
                run = 15u;
            else if (clip != RLE_CLIP_NONE && run == 0u)
                run = 1u;

            for (uint32_t i = 0; i < run; ++i) {
                if (clip == RLE_CLIP_NONE ||
                    (clip == RLE_CLIP_LEFT ? cutoff <= 0 : cutoff > 0)) {
                    rle_write_pixel(destination, source, operation, value);
                }
                source += 2;
                destination += 2;
                if (clip != RLE_CLIP_NONE) --cutoff;
                --remaining;
            }
        }

        row_destination = clip == RLE_CLIP_NONE
            ? destination + target->destination_row_skip_bytes
            : row_destination + target->surface_pitch_bytes;
    } while (--rows > 0);
}

/* Complete one-callee C translations of Pharaoh.exe 0x00410230,
 * 0x004105C0, 0x00410660, 0x00410700, 0x004109D0, 0x00410A70,
 * 0x00410B10, 0x00410DE0 and 0x00410E80. */
void pharaoh_image_blit_compressed(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels)
{ rle_blit(target, pixels, RLE_CLIP_NONE, RLE_COPY, 0); }

void pharaoh_image_blit_compressed_left(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels)
{ rle_blit(target, pixels, RLE_CLIP_LEFT, RLE_COPY, 0); }

void pharaoh_image_blit_compressed_right(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels)
{ rle_blit(target, pixels, RLE_CLIP_RIGHT, RLE_COPY, 0); }

void pharaoh_image_blit_compressed_mask(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels,
    uint16_t mask)
{ rle_blit(target, pixels, RLE_CLIP_NONE, RLE_MASK, mask); }

void pharaoh_image_blit_compressed_mask_left(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels,
    uint16_t mask)
{ rle_blit(target, pixels, RLE_CLIP_LEFT, RLE_MASK, mask); }

void pharaoh_image_blit_compressed_mask_right(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels,
    uint16_t mask)
{ rle_blit(target, pixels, RLE_CLIP_RIGHT, RLE_MASK, mask); }

void pharaoh_image_blit_compressed_color(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels)
{ rle_blit(target, pixels, RLE_CLIP_NONE, RLE_SOLID, target->solid_color); }

void pharaoh_image_blit_compressed_color_left(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels)
{ rle_blit(target, pixels, RLE_CLIP_LEFT, RLE_SOLID, target->solid_color); }

void pharaoh_image_blit_compressed_color_right(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels)
{ rle_blit(target, pixels, RLE_CLIP_RIGHT, RLE_SOLID, target->solid_color); }

static void rle_blit_mirrored(const PharaohSpriteRleBlitTarget *target,
    const uint8_t *pixels, int32_t clip)
{
    const uint8_t *source = pharaoh_graphics_rle_skip_rows(pixels,
        target->source_rows_to_skip, target->image_width);
    uint8_t *row_base = rle_destination_start(target);
    int32_t rows = target->visible_height;

    if (rows <= 0) return;
    do {
        int32_t remaining = target->image_width;
        int32_t cutoff = 0;
        uint8_t *destination;

        if (clip == RLE_CLIP_RIGHT) {
            cutoff = remaining - target->right_visible_width;
            destination = row_base + (ptrdiff_t)target->image_width * 2;
        } else if (clip == RLE_CLIP_LEFT) {
            const uint32_t raw = (uint32_t)target->left_clip_bytes;
            const int32_t clip_pixels =
                (int32_t)((raw >> 1) | (raw & 0x80000000u));
            cutoff = remaining - clip_pixels;
            destination = row_base + (ptrdiff_t)target->image_width * 2 -
                target->left_clip_bytes;
        } else {
            destination = row_base + (ptrdiff_t)target->image_width * 2;
        }

        while (remaining > 0) {
            uint32_t run = *source++;
            if (run == 0xffu) {
                run = *source++;
                destination -= run * 2u;
                remaining -= (int32_t)run;
                if (clip != RLE_CLIP_NONE) cutoff -= (int32_t)run;
                continue;
            }
            while (run != 0u) {
                if (clip == RLE_CLIP_NONE && remaining == 0) break;
                if (clip == RLE_CLIP_NONE ||
                    (clip == RLE_CLIP_LEFT ? cutoff > 0 : cutoff <= 0)) {
                    memcpy(destination, source, sizeof(uint16_t));
                }
                destination -= 2;
                source += 2;
                --remaining;
                if (clip != RLE_CLIP_NONE) --cutoff;
                --run;
            }
        }

        row_base = clip == RLE_CLIP_NONE
            ? destination + target->destination_row_skip_bytes +
                (ptrdiff_t)target->image_width * 2
            : row_base + target->surface_pitch_bytes;
    } while (--rows != 0);
}

/* Complete one-callee C translations of Pharaoh.exe 0x0040FC40,
 * 0x0040FCF0 and 0x0040FDD0. */
void pharaoh_image_blit_compressed_mirrored(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels)
{ rle_blit_mirrored(target, pixels, RLE_CLIP_NONE); }

void pharaoh_image_blit_compressed_mirrored_left(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels)
{ rle_blit_mirrored(target, pixels, RLE_CLIP_LEFT); }

void pharaoh_image_blit_compressed_mirrored_right(
    const PharaohSpriteRleBlitTarget *target, const uint8_t *pixels)
{ rle_blit_mirrored(target, pixels, RLE_CLIP_RIGHT); }
