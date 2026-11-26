#ifndef PHARAOH_UI_BITMAP_FONT_H
#define PHARAOH_UI_BITMAP_FONT_H
#include "pharaoh/assets/sprite_libraries.h"

typedef struct PharaohTextSelection PharaohTextSelection;

typedef struct PharaohBitmapFont {
    const PharaohSpriteImageRecord *images;
    uint32_t first_image, image_count;
    int32_t measure_space, draw_space, measure_gap, draw_gap, baseline;
    void *user_data;
    uint32_t *current_glyph;
    void (*draw_glyph)(void *, uint32_t image, int32_t x, int32_t y, uint16_t color);
} PharaohBitmapFont;

typedef struct PharaohBitmapTextDrawState {
    int32_t cursor_x, cursor_y;
    int32_t encoded_character_index;
    uint32_t hash_mode, character_index;
    uint32_t anchor_captured;
    int32_t anchor_advance, anchor_x, anchor_y;
    uint8_t previous_space, duplicate_space;
    uint32_t cursor_slot;
    const int32_t *cursor_limits;
    uint32_t outline, vertical_shadow, diagonal_shadow;
    uint32_t *current_glyph;
    uint16_t color;
    uint32_t reset_flag;
    uint32_t font_id, font_id_large_a, font_id_large_b, font_id_small;
    int32_t baseline;
} PharaohBitmapTextDrawState;

/* Complete width owner 0x004CBF30 and glyph renderer 0x004CCA60. */
int32_t pharaoh_bitmap_text_width(const PharaohBitmapFont *font, const uint8_t *text);
int32_t pharaoh_bitmap_text_width_native(const PharaohBitmapFont *font,
    const uint8_t *text,const uint8_t glyph_map[256],uint32_t *current_glyph);
int32_t pharaoh_bitmap_text_entry_width(const PharaohBitmapFont *font,
    PharaohTextSelection *selection, int32_t group, int32_t entry);
int32_t pharaoh_bitmap_text_entry_width_native(const PharaohBitmapFont *font,
    PharaohTextSelection *selection,int32_t group,int32_t entry,
    const uint8_t glyph_map[256],uint32_t *current_glyph);
int32_t pharaoh_bitmap_text_draw_advance(const PharaohBitmapFont *font,
    const uint8_t *text);
/* Complete typed C translation of native glyph renderer 0x004CCA60.
   `character_index` is the source byte minus 32 (0..223); `current_glyph`
   receives the native shared glyph value, including zero for a missing glyph. */
int32_t pharaoh_bitmap_font_draw_glyph(const PharaohBitmapFont *font,
    uint8_t character_index, int32_t x, int32_t y, uint16_t color,
    uint32_t *current_glyph);
/* Full native text state machine at 0x004CC770. Cursor limits are required
   caller-owned native state, not synthesized defaults. */
void pharaoh_bitmap_text_draw_native(const PharaohBitmapFont *font,
    PharaohBitmapTextDrawState *state, const uint8_t *text,
    int32_t x, int32_t y, uint16_t color);
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
