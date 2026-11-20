#ifndef PHARAOH_UI_BITMAP_FONT_H
#define PHARAOH_UI_BITMAP_FONT_H
#include "pharaoh/assets/sprite_libraries.h"

typedef struct PharaohBitmapFont {
    const PharaohSpriteImageRecord *images;
    uint32_t first_image, image_count;
    int32_t measure_space, draw_space, measure_gap, draw_gap, baseline;
    void *user_data;
    void (*draw_glyph)(void *, uint32_t image, int32_t x, int32_t y, uint16_t color);
} PharaohBitmapFont;

/* Startup-label subset of native 004CBF30/004CC720/004CC770/004CCA60.
   Includes native glyph IDs, spacing and accent positioning. Interactive
   text-input side effects, outlines and hash substitutions are separate owners. */
int32_t pharaoh_bitmap_text_width(const PharaohBitmapFont *font, const uint8_t *text);
void pharaoh_bitmap_text_draw_at(const PharaohBitmapFont *font,
    const uint8_t *text, int32_t x, int32_t y, uint16_t color);
void pharaoh_bitmap_text_draw_centered(const PharaohBitmapFont *font,
    const uint8_t *text, int32_t x, int32_t y, int32_t width, uint16_t color);
/* Complete translations of native 0x004CD120 and 0x004CD010. */
int32_t pharaoh_bitmap_wrapped_word_width(const PharaohBitmapFont *font,
    const uint8_t *text, int32_t *byte_count);
void pharaoh_bitmap_text_draw_wrapped(const PharaohBitmapFont *font,
    const uint8_t *text, int32_t x, int32_t y, int32_t width,
    uint16_t color);
#endif
