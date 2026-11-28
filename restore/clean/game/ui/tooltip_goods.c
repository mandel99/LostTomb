#include "pharaoh/ui/tooltip_goods.h"

#include <stdint.h>

enum {
    TOOLTIP_MAP_FLAG_WALKER = 36,
    TOOLTIP_SMALL_HEIGHT = 20,
    TOOLTIP_SMALL_PADDING = 5,
    TOOLTIP_MOUSE_GAP = 20,
    TOOLTIP_LOWER_HALF_Y = 200,
    TOOLTIP_BELOW = 10,
    TOOLTIP_ABOVE = 32,
    TOOLTIP_LUXURY_KIND_WIDTH = 60,
    TOOLTIP_LUXURY_EXTRA_WIDTH = 50,
    TOOLTIP_LUXURY_TEXT_GROUP = 23,
    TOOLTIP_LUXURY_TEXT_FIRST = 39,
    TOOLTIP_WALKER_TEXT_GROUP = 48,
    TOOLTIP_WALKER_GRAIN_TEXT_GROUP = 49,
    DISPLAY_MODE_RGB565 = 2,
    SMALL_FILL_RGB565 = 0xbb88,
    SMALL_FRAME_RGB565 = 0x58a0,
    SMALL_TEXT_RGB565 = 0x3922,
    SMALL_FILL_RGB555 = 0x5dc8,
    SMALL_FRAME_RGB555 = 0x2c40,
    SMALL_TEXT_RGB555 = 0x1c82
};

static int32_t add32(int32_t left, int32_t right)
{
    return (int32_t)((uint32_t)left + (uint32_t)right);
}

static void draw_small_box(const PharaohTooltipGoodsDrawing *drawing,
    int32_t width)
{
    int32_t fill;
    int32_t frame;

    *drawing->box_width = width;
    *drawing->box_height = TOOLTIP_SMALL_HEIGHT;
    if (*drawing->mouse_x < add32(width, TOOLTIP_MOUSE_GAP))
        *drawing->box_x = add32(*drawing->mouse_x, TOOLTIP_MOUSE_GAP);
    else
        *drawing->box_x = add32(add32(*drawing->mouse_x, -width),
            -TOOLTIP_MOUSE_GAP);
    *drawing->box_y = *drawing->mouse_y < TOOLTIP_LOWER_HALF_Y ?
        add32(*drawing->mouse_y, TOOLTIP_BELOW) :
        add32(*drawing->mouse_y, -TOOLTIP_ABOVE);

    if (*drawing->display_mode == DISPLAY_MODE_RGB565) {
        *drawing->draw_color = SMALL_TEXT_RGB565;
        fill = SMALL_FILL_RGB565;
        frame = SMALL_FRAME_RGB565;
    } else {
        *drawing->draw_color = SMALL_TEXT_RGB555;
        fill = SMALL_FILL_RGB555;
        frame = SMALL_FRAME_RGB555;
    }
    pharaoh_graphics_draw_rect(drawing->graphics, *drawing->box_x,
        *drawing->box_y, width, TOOLTIP_SMALL_HEIGHT, frame);
    pharaoh_graphics_fill_rect(drawing->graphics,
        add32(*drawing->box_x, 1), add32(*drawing->box_y, 1),
        add32(*drawing->box_width, -2),
        add32(*drawing->box_height, -2), fill);
}

static int32_t walker_goods_text(uint8_t goods, int32_t *group)
{
    *group = TOOLTIP_WALKER_TEXT_GROUP;
    if (goods == 1) {
        *group = TOOLTIP_WALKER_GRAIN_TEXT_GROUP;
        return 3;
    }
    if (goods == 2) return 5;
    if (goods == 3) return 6;
    if (goods == 12) return 18;
    if (goods == 13) return 19;
    if (goods >= 14 && goods <= 25) return (int32_t)goods + 9;
    if (goods >= 26 && goods <= 29) return (int32_t)goods + 13;
    if (goods >= 30 && goods <= 34) return (int32_t)goods + 26;
    if ((int32_t)goods - 50 < 8) return (int32_t)goods - 40;
    return (int32_t)goods - 10;
}

void pharaoh_tooltip_draw_walker_goods(
    const PharaohWalkerGoodsTooltipContext *context)
{
    const PharaohTooltipGoodsDrawing *drawing = context->drawing;
    int32_t walker_id = context->tile_walker_ids[*context->selected_tile];
    const PharaohWalkerQueryRecord *walker;
    const uint8_t *text;
    int32_t group;
    int32_t entry;
    int32_t width;
    uint16_t saved_color;

    if (walker_id == 0)
        return;
    walker = &context->walkers[walker_id];
    while (walker->type != TOOLTIP_MAP_FLAG_WALKER) {
        walker_id = walker->next_walker_id_on_same_tile;
        if (walker_id == 0)
            return;
        walker = &context->walkers[walker_id];
    }

    entry = walker_goods_text(walker->resource_id, &group);
    pharaoh_text_select(drawing->text_selection, group, entry);
    text = (const uint8_t *)drawing->text_selection->current;
    width = add32(pharaoh_bitmap_text_width_native(drawing->font, text,
        drawing->glyph_map, drawing->current_glyph), TOOLTIP_MOUSE_GAP);
    saved_color = *drawing->draw_color;
    draw_small_box(drawing, width);
    drawing->text_state->cursor_x = 0;
    pharaoh_bitmap_text_draw_native(drawing->font, drawing->text_state, text,
        add32(*drawing->box_x, TOOLTIP_SMALL_PADDING),
        add32(*drawing->box_y, TOOLTIP_SMALL_PADDING),
        *drawing->draw_color);
    *drawing->draw_color = saved_color;
}

void pharaoh_tooltip_draw_luxury_goods(
    const PharaohLuxuryGoodsTooltipContext *context)
{
    const PharaohTooltipGoodsDrawing *drawing = context->drawing;
    int32_t kinds[PHARAOH_LUXURY_KIND_COUNT] = {0};
    const int32_t count = pharaoh_empire_count_luxury_goods_kinds(
        context->luxury_goods, kinds);
    const int32_t width = add32(count * TOOLTIP_LUXURY_KIND_WIDTH,
        TOOLTIP_LUXURY_EXTRA_WIDTH);
    uint16_t saved_color = *drawing->draw_color;
    int32_t drawn = 0;
    int32_t kind;

    draw_small_box(drawing, width);
    drawing->text_state->cursor_x = 0;
    for (kind = 0; kind < PHARAOH_LUXURY_KIND_COUNT; ++kind) {
        int32_t x;
        int32_t y;
        if (kinds[kind] == 0)
            continue;
        x = add32(add32(drawing->text_state->cursor_x,
            *drawing->box_x), TOOLTIP_SMALL_PADDING);
        y = add32(*drawing->box_y, TOOLTIP_SMALL_PADDING);
        if (drawn != 0) {
            pharaoh_bitmap_text_draw_native(drawing->font,
                drawing->text_state, context->comma_text, x, y,
                *drawing->draw_color);
            x = add32(add32(drawing->text_state->cursor_x,
                *drawing->box_x), TOOLTIP_SMALL_PADDING);
        }
        pharaoh_bitmap_text_draw_selection_native(drawing->font,
            drawing->text_state, drawing->text_selection,
            TOOLTIP_LUXURY_TEXT_GROUP, kind + TOOLTIP_LUXURY_TEXT_FIRST,
            x, y, *drawing->font_id, *drawing->draw_color);
        ++drawn;
    }
    *drawing->draw_color = saved_color;
}
