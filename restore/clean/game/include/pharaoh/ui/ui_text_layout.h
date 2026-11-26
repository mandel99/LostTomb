#ifndef PHARAOH_UI_UI_TEXT_LAYOUT_H
#define PHARAOH_UI_UI_TEXT_LAYOUT_H

#include "pharaoh/ui/window_layout.h"
#include "pharaoh/ui/button_layout.h"

typedef struct PharaohUiTextLayoutContext {
    const PharaohUiLayoutContext *layout;
    PharaohLayoutHighlightState *highlight;
    const uint8_t *window_id;
    const int32_t *window_seven_layout_base;
    PharaohTextSelection *selection;
    PharaohBitmapTextDrawState *drawing;
    const uint8_t *glyph_map;
    const char **current_text;
    int32_t *center_offset;
    uint32_t *reset_flag;
    char *number_text; /* native 256-byte formatter buffer */
} PharaohUiTextLayoutContext;

/* Complete 0x004D62F0, 0x004D6250 and 0x004D6860. font is the native
   font_id's asset view. current_text aliases selection->current and
   reset_flag aliases drawing->reset_flag; glyph state is shared. */
void pharaoh_ui_text_draw_language(const PharaohUiTextLayoutContext *,
    const PharaohBitmapFont *font, int32_t group, int32_t entry,
    int32_t x, int32_t y, int32_t font_id, uint16_t color,
    int32_t origin_x, int32_t origin_y);
void pharaoh_ui_text_draw_centered_language(const PharaohUiTextLayoutContext *,
    const PharaohBitmapFont *font, int32_t group, int32_t entry,
    int32_t x, int32_t y, int32_t width, int32_t font_id, uint16_t color,
    int32_t origin_x, int32_t origin_y);
void pharaoh_ui_text_draw_number(const PharaohUiTextLayoutContext *,
    const PharaohBitmapFont *font, int32_t value, char prefix,
    const char *suffix, int32_t x, int32_t y, int32_t font_id,
    uint16_t color, int32_t origin_x, int32_t origin_y);

#endif
