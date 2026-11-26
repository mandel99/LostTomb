#include "pharaoh/ui/window_layout_key.h"
#include "pharaoh/runtime/text_loading.h"

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_WINDOW_LAYOUT_SPECIAL_KEY = 7,
    PHARAOH_WINDOW_LAYOUT_KEY_OFFSET = 100
};

static int32_t wrap_add(int32_t left, int32_t right)
{
    const uint32_t result_bits = (uint32_t)left + (uint32_t)right;
    if ((result_bits & UINT32_C(0x80000000)) != 0) {
        return (int32_t)((int64_t)result_bits - INT64_C(0x100000000));
    }
    return (int32_t)result_bits;
}

int32_t pharaoh_ui_layout_window_key(int32_t window_key,
    int32_t window_seven_layout_base)
{
    if (window_key == PHARAOH_WINDOW_LAYOUT_SPECIAL_KEY) {
        return wrap_add(window_seven_layout_base,
            PHARAOH_WINDOW_LAYOUT_KEY_OFFSET);
    }
    return window_key;
}

int32_t pharaoh_ui_layout_group_max_width(
    const PharaohUiLayoutTextContext *context, int32_t group)
{
    int32_t maximum = 1;
    int32_t entry = 1;
    pharaoh_text_select(context->selection, wrap_add(group, 1), 0);
    *context->current_text_00e20768 = context->selection->current;
    const char *const end = *context->current_text_00e20768;
    pharaoh_text_select(context->selection, group, entry);
    *context->current_text_00e20768 = context->selection->current;
    do {
        const int32_t width = pharaoh_bitmap_text_width_native(context->font,
            (const uint8_t *)*context->current_text_00e20768,
            context->glyph_map, context->current_glyph);
        if (maximum < width) maximum = width;
        entry = wrap_add(entry, 1);
        pharaoh_text_select(context->selection, group, entry);
        *context->current_text_00e20768 = context->selection->current;
    } while (*context->current_text_00e20768 != end);
    return maximum;
}

void pharaoh_ui_layout_init_column_widths(
    const PharaohUiLayoutTextContext *context, int32_t columns[6])
{
    static const int32_t groups[6] = {1, 2, 3, 4, 7, 10};
    for (int32_t column = 0; column < 6; ++column) {
        columns[column] = pharaoh_ui_layout_group_max_width(context,
            groups[column]) / 16 + 2;
    }
}

void pharaoh_ui_layout_init_label_width(
    const PharaohUiLayoutTextContext *context, PharaohGenericButton buttons[30],
    int32_t *label_columns, int32_t *label_width)
{
    const int32_t width = pharaoh_ui_layout_group_max_width(context, 0x1c);
    const int32_t columns = width / 16 + (width % 16 != 0);
    const uint16_t right = (uint16_t)(((uint32_t)columns + 7u) * 16u);
    *label_columns = columns;
    for (int32_t button = 0; button < 30; ++button) {
        buttons[button].bounds.right = (int16_t)right;
    }
    *label_width = (int32_t)((uint32_t)columns * 16u);
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004D5FE0. */
PharaohUiLayoutNode *pharaoh_ui_layout_find(
    const PharaohUiLayoutEntry *entries, int32_t kind, int32_t mode,
    int32_t relative_x, int32_t relative_y)
{
    int32_t list;
    PharaohUiLayoutNode *node;
    if (kind == 1) list = 0;
    else if (kind == 2) list = 1;
    else if (kind == 3 || kind == 4) list = 2;
    else return NULL;
    node = entries[mode].lists[list];
    while (node != NULL) {
        if (node->relative_x == relative_x && node->relative_y == relative_y)
            return node;
        node = node->next;
    }
    return NULL;
}
