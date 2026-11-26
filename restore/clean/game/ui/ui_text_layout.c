#include "pharaoh/ui/ui_text_layout.h"
#include <stddef.h>

static int32_t add(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a + (uint32_t)b); }
static int32_t sub(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a - (uint32_t)b); }

static void prepare(const PharaohUiTextLayoutContext *context,
    int32_t x, int32_t y, int32_t origin_x, int32_t origin_y,
    int32_t *relative_x, int32_t *relative_y, int32_t *width,
    int32_t replace_width)
{
    if (context->layout->state->initialized == 0) {
        pharaoh_ui_layout_initialize(context->layout);
    }
    *relative_x = sub(x, origin_x);
    *relative_y = sub(y, origin_y);
    const int32_t window = *context->window_id;
    const int32_t key = pharaoh_ui_layout_window_key(window,
        window == 7 ? *context->window_seven_layout_base : 0);
    PharaohUiLayoutNode *const node = pharaoh_ui_layout_find(
        context->layout->state->entries, 1, key, *relative_x, *relative_y);
    if (node != NULL) {
        *relative_x = node->values_08[0];
        *relative_y = node->values_08[1];
        if (replace_width) *width = node->values_08[2];
    }
    pharaoh_ui_layout_highlight(context->highlight, 1, x, y, *width, 24,
        *relative_x, *relative_y, origin_x, origin_y);
}

void pharaoh_ui_text_draw_language(const PharaohUiTextLayoutContext *context,
    const PharaohBitmapFont *font, int32_t group, int32_t entry,
    int32_t x, int32_t y, int32_t font_id, uint16_t color,
    int32_t origin_x, int32_t origin_y)
{
    int32_t relative_x, relative_y, width = 100;
    prepare(context, x, y, origin_x, origin_y,
        &relative_x, &relative_y, &width, 0);
    pharaoh_bitmap_text_draw_selection_native(font, context->drawing,
        context->selection, group, entry, add(relative_x, origin_x),
        add(relative_y, origin_y), font_id, color);
}

void pharaoh_ui_text_draw_centered_language(
    const PharaohUiTextLayoutContext *context, const PharaohBitmapFont *font,
    int32_t group, int32_t entry, int32_t x, int32_t y, int32_t width,
    int32_t font_id, uint16_t color, int32_t origin_x, int32_t origin_y)
{
    int32_t relative_x, relative_y;
    prepare(context, x, y, origin_x, origin_y,
        &relative_x, &relative_y, &width, 1);
    const PharaohBitmapTextCenteredNativeContext drawing = {
        font, context->drawing, context->glyph_map, context->center_offset
    };
    const PharaohBitmapTextCatalogCenteredNativeContext catalog = {
        context->selection, &drawing, context->current_text, context->reset_flag
    };
    pharaoh_bitmap_text_draw_catalog_centered_native(&catalog, group, entry,
        add(relative_x, origin_x), add(relative_y, origin_y), width,
        font_id, color);
}

void pharaoh_ui_text_draw_number(const PharaohUiTextLayoutContext *context,
    const PharaohBitmapFont *font, int32_t value, char prefix,
    const char *suffix, int32_t x, int32_t y, int32_t font_id,
    uint16_t color, int32_t origin_x, int32_t origin_y)
{
    int32_t relative_x, relative_y, width = 24;
    prepare(context, x, y, origin_x, origin_y,
        &relative_x, &relative_y, &width, 0);
    pharaoh_bitmap_text_draw_number_native(font, context->drawing,
        context->number_text, value, prefix, suffix,
        add(relative_x, origin_x), add(relative_y, origin_y), font_id, color);
}
