#include "pharaoh/assets/sprite_pixel_conversion.h"

#include <string.h>

static uint16_t read_pixel(const uint8_t *bytes)
{
    uint16_t pixel;
    memcpy(&pixel, bytes, sizeof(pixel));
    return pixel;
}

static void write_pixel(uint8_t *bytes, uint16_t pixel)
{
    memcpy(bytes, &pixel, sizeof(pixel));
}

static uint16_t convert_pixel(uint16_t pixel, uint16_t green_mask)
{
    return (uint16_t)(((uint32_t)(pixel & green_mask) << 1) |
        (pixel & UINT16_C(0x001f)));
}

void pharaoh_sprite_convert_raw_555_to_565(int32_t pixel_mode,
    uint8_t *pixels, int32_t byte_count)
{
    if (pixel_mode != PHARAOH_SPRITE_PIXEL_MODE_RGB565) {
        return;
    }

    while (byte_count > 0) {
        const uint16_t pixel = read_pixel(pixels);
        if (pixel != UINT16_C(0xf81f)) {
            write_pixel(pixels, convert_pixel(pixel, UINT16_C(0xffe0)));
        }
        pixels += sizeof(uint16_t);
        byte_count -= (int32_t)sizeof(uint16_t);
    }
}

void pharaoh_sprite_convert_compressed_555_to_565(int32_t pixel_mode,
    uint8_t *pixels, int32_t byte_count)
{
    if (pixel_mode != PHARAOH_SPRITE_PIXEL_MODE_RGB565) {
        return;
    }

    while (byte_count != 0 && byte_count >= 0) {
        const int8_t run_length = (int8_t)pixels[0];
        if (run_length == -1) {
            pixels += 2;
            byte_count -= 2;
        } else {
            ++pixels;
            --byte_count;
            for (int32_t remaining = run_length; remaining > 0; --remaining) {
                const uint16_t pixel = read_pixel(pixels);
                write_pixel(pixels, convert_pixel(pixel, UINT16_C(0x7fe0)));
                pixels += sizeof(uint16_t);
                byte_count -= (int32_t)sizeof(uint16_t);
            }
        }
    }
}
