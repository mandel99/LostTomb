#ifndef PHARAOH_ASSETS_SPRITE_PIXEL_CONVERSION_H
#define PHARAOH_ASSETS_SPRITE_PIXEL_CONVERSION_H

#include <stdint.h>

enum {
    PHARAOH_SPRITE_PIXEL_MODE_RGB565 = 2
};

void pharaoh_sprite_convert_raw_555_to_565(int32_t pixel_mode,
    uint8_t *pixels, int32_t byte_count);
void pharaoh_sprite_convert_compressed_555_to_565(int32_t pixel_mode,
    uint8_t *pixels, int32_t byte_count);

#endif
