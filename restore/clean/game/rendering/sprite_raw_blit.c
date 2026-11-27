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

static void mask_keyed_pixel(
    uint8_t *destination, const uint8_t *source, uint16_t mask)
{
    uint16_t source_pixel;
    uint16_t destination_pixel;
    memcpy(&source_pixel, source, sizeof(source_pixel));
    if (source_pixel != SPRITE_RAW_TRANSPARENT_COLOR) {
        memcpy(&destination_pixel, destination, sizeof(destination_pixel));
        destination_pixel &= mask;
        memcpy(destination, &destination_pixel, sizeof(destination_pixel));
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

/* Complete C translation of Pharaoh.exe 0x0040FED0. */
void pharaoh_sprite_raw_blit_mirrored(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels)
{
    uint8_t *destination = destination_start(target);
    const uint8_t *source = source_pixels;
    const int32_t width_bytes = target->width * (int32_t)sizeof(uint16_t);
    int32_t row;
    if (target->height <= 0) return;
    for (row = 0; row < target->height; ++row) {
        int32_t column = 0;
        destination += width_bytes;
        while (column < target->width) {
            copy_keyed_pixel(destination, source);
            destination -= sizeof(uint16_t);
            source += sizeof(uint16_t);
            ++column;
        }
        destination += target->destination_row_skip_bytes + width_bytes;
    }
}

/* Complete C translation of Pharaoh.exe 0x0040FF50. */
void pharaoh_sprite_raw_blit_mirrored_clipped(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels)
{
    uint8_t *destination = destination_start(target);
    const uint8_t *source = source_pixels;
    const uint32_t width_bytes = (uint32_t)target->width * sizeof(uint16_t);
    uint32_t rows = (uint32_t)target->height;
    if (rows == 0) return;
    do {
        uint32_t columns = (uint32_t)target->width;
        destination += width_bytes;
        while (columns != 0) {
            copy_keyed_pixel(destination, source);
            destination -= sizeof(uint16_t);
            source += sizeof(uint16_t);
            --columns;
        }
        source += target->source_row_skip_bytes;
        destination += target->destination_row_skip_bytes + width_bytes;
        --rows;
    } while (rows != 0);
}

/* Complete C translation of Pharaoh.exe 0x004100F0. */
void pharaoh_sprite_raw_mask(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels,
    uint16_t mask)
{
    uint8_t *destination = destination_start(target);
    const uint8_t *source = source_pixels;
    int32_t rows = target->height;
    do {
        uint32_t remaining = (uint32_t)target->width;
        int32_t index;
        if ((remaining & 3u) == 0u) {
            for (index = 0; index < 4; ++index) {
                mask_keyed_pixel(destination, source, mask);
                source += sizeof(uint16_t);
                destination += sizeof(uint16_t);
            }
            remaining -= 4u;
        } else {
            mask_keyed_pixel(destination, source, mask);
            source += sizeof(uint16_t);
            destination += sizeof(uint16_t);
            --remaining;
        }
        while ((int32_t)remaining > 0) {
            mask_keyed_pixel(destination, source, mask);
            source += sizeof(uint16_t);
            destination += sizeof(uint16_t);
            --remaining;
        }
        destination += target->destination_row_skip_bytes;
        --rows;
    } while (rows > 0);
}

/* Complete C translation of Pharaoh.exe 0x004101C0. */
void pharaoh_sprite_raw_mask_clipped(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels,
    uint16_t mask)
{
    uint8_t *destination = destination_start(target);
    const uint8_t *source = source_pixels;
    int32_t rows = target->height;
    do {
        int32_t remaining = target->width;
        do {
            mask_keyed_pixel(destination, source, mask);
            source += sizeof(uint16_t);
            destination += sizeof(uint16_t);
            --remaining;
        } while (remaining > 0);
        source += target->source_row_skip_bytes;
        destination += target->destination_row_skip_bytes;
        --rows;
    } while (rows > 0);
}

static void blit_opaque_square(const PharaohSpriteRawBlitTarget *target,
    const uint8_t *source_pixels, int32_t size)
{
    uint8_t *destination = destination_start(target);
    const uint8_t *source = source_pixels;
    int32_t row;
    for (row = 0; row < size; ++row) {
        int32_t column;
        for (column = 0; column < size / 2; ++column) {
            uint32_t pixels;
            memcpy(&pixels, source, sizeof(pixels));
            memcpy(destination + column * sizeof(pixels),
                &pixels, sizeof(pixels));
            source += sizeof(pixels);
        }
        destination += target->surface_pitch_bytes;
    }
}

/* Complete C translations of Pharaoh.exe 0x00410FB0, 0x004114A0 and
 * 0x00411FD0. */
void pharaoh_sprite_raw_blit_square16(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels)
{
    blit_opaque_square(target, source_pixels, 16);
}

void pharaoh_sprite_raw_blit_square24(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels)
{
    blit_opaque_square(target, source_pixels, 24);
}

void pharaoh_sprite_raw_blit_square32(
    const PharaohSpriteRawBlitTarget *target, const uint8_t *source_pixels)
{
    blit_opaque_square(target, source_pixels, 32);
}

/* Complete C translation of Pharaoh.exe 0x00410F20. */
const uint8_t *pharaoh_graphics_rle_skip_rows(
    const uint8_t *source, int32_t row_count, int32_t row_width)
{
    while (row_count > 0) {
        int32_t remaining = row_width;
        --row_count;
        while (remaining > 0) {
            const uint32_t count = source[0];
            if (count == 0xffu) {
                remaining -= source[1];
                source += 2;
            } else {
                remaining -= (int32_t)count;
                source += count * 2u + 1u;
            }
        }
    }
    return source;
}

void pharaoh_graphics_and_bytes(
    uint32_t *data, int32_t byte_count, uint8_t mask)
{
    const uint32_t repeated = (uint32_t)mask * UINT32_C(0x01010101);
    uint8_t *bytes;
    while (byte_count > 63) {
        int32_t index;
        for (index = 0; index < 16; ++index) data[index] &= repeated;
        data += 16;
        byte_count -= 64;
    }
    bytes = (uint8_t *)data;
    while (byte_count > 0) {
        *bytes++ &= mask;
        --byte_count;
    }
}

void pharaoh_graphics_draw_from_buffer(
    const PharaohSpriteRawBlitTarget *target, const uint32_t *source)
{
    uint8_t *destination = destination_start(target);
    int32_t rows = target->height;
    const size_t row_bytes = (size_t)target->width * 2u * sizeof(uint32_t);
    do {
        memcpy(destination, source, row_bytes);
        destination += row_bytes + target->destination_row_skip_bytes;
        source = (const uint32_t *)((const uint8_t *)source +
            row_bytes + target->source_row_skip_bytes);
        --rows;
    } while (rows > 0);
}

void pharaoh_graphics_save_to_buffer(
    const PharaohSpriteRawBlitTarget *target, uint32_t *destination)
{
    const uint8_t *source = destination_start(target);
    int32_t rows = target->height;
    const size_t row_bytes = (size_t)target->width * 2u * sizeof(uint32_t);
    do {
        memcpy(destination, source, row_bytes);
        source += row_bytes + target->destination_row_skip_bytes;
        destination = (uint32_t *)((uint8_t *)destination +
            row_bytes + target->source_row_skip_bytes);
        --rows;
    } while (rows > 0);
}
