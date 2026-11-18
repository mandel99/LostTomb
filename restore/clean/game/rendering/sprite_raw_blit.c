#include "pharaoh/rendering/sprite_raw_blit.h"

#include <stddef.h>
#include <string.h>

enum { SPRITE_RAW_TRANSPARENT_COLOR = 0xf81f };

static uint8_t *destination_start(const PharaohSpriteRawBlitTarget *target)
{
    return target->surface_pixels +
        (ptrdiff_t)target->y * target->surface_pitch_bytes +
        (ptrdiff_t)target->x * (ptrdiff_t)sizeof(uint16_t);
}

static void copy_keyed_pixel(uint8_t *destination, const uint8_t *source)
{
    uint16_t pixel;
    memcpy(&pixel, source, sizeof(pixel));
    if (pixel != SPRITE_RAW_TRANSPARENT_COLOR) {
        memcpy(destination, &pixel, sizeof(pixel));
    }
}

void pharaoh_sprite_raw_blit_keyed(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels)
{
    uint8_t *destination = destination_start(target);
    const uint8_t *source = source_pixels;
    int32_t rows = target->height;

    do {
        if (target->width == 0) {
            /* Native's four-pixel fast path writes a group when width is 0. */
            for (int32_t column = 0; column < 4; ++column) {
                copy_keyed_pixel(destination, source);
                destination += sizeof(uint16_t);
                source += sizeof(uint16_t);
            }
        } else {
            for (int32_t column = 0; column < target->width; ++column) {
                copy_keyed_pixel(destination, source);
                destination += sizeof(uint16_t);
                source += sizeof(uint16_t);
            }
        }
        destination += target->destination_row_skip_bytes;
        --rows;
    } while (rows > 0);
}

void pharaoh_sprite_raw_blit_keyed_clipped(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels)
{
    uint8_t *destination = destination_start(target);
    const uint8_t *source = source_pixels;
    int32_t rows = target->height;

    do {
        int32_t columns = target->width > 0 ? target->width : 1;
        do {
            copy_keyed_pixel(destination, source);
            destination += sizeof(uint16_t);
            source += sizeof(uint16_t);
            --columns;
        } while (columns > 0);

        if (rows > 1) {
            source += target->source_row_skip_bytes;
            destination += target->destination_row_skip_bytes;
        }
        --rows;
    } while (rows > 0);
}

void pharaoh_sprite_raw_blit_clipped(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels)
{
    uint8_t *destination = destination_start(target);
    const uint8_t *source = source_pixels;
    int32_t rows = target->height;

    do {
        for (int32_t column = 0; column < target->width; ++column) {
            memcpy(destination, source, sizeof(uint16_t));
            destination += sizeof(uint16_t);
            source += sizeof(uint16_t);
        }

        if (rows > 1) {
            source += target->source_row_skip_bytes;
            destination += target->destination_row_skip_bytes;
        }
        --rows;
    } while (rows > 0);
}
