#include "pharaoh/ui/tooltip_text_panels.h"

#include <stdint.h>

enum {
    TOOLTIP_TEXT_WIDTH = 220,
    TOOLTIP_TEXT_INNER_WIDTH = 190,
    TOOLTIP_TEXT_WIDE_WIDTH = 300,
    TOOLTIP_TEXT_WIDE_INNER_WIDTH = 270,
    TOOLTIP_TEXT_INSET = 30,
    TOOLTIP_BORDER = 20,
    TOOLTIP_PADDING = 5,
    TOOLTIP_TEXT_TOP = 7,
    TOOLTIP_LINE_HEIGHT = 16,
    TOOLTIP_EXTRA_HEIGHT = 10,
    TOOLTIP_VIEWPORT_LINES = 30,
    TOOLTIP_NARROW_MAX_LINES = 2,
    TOOLTIP_MOUSE_GAP = 20,
    TOOLTIP_BELOW = 50,
    TOOLTIP_ABOVE = 72,
    TOOLTIP_LOWER_HALF_Y = 200,
    TOOLTIP_MEASURE_COLOR = 0x4208,
    TOOLTIP_SENATE_WIDTH = 180,
    TOOLTIP_SENATE_HEIGHT = 120,
    TOOLTIP_SENATE_RIGHT_EDGE = 200,
    TOOLTIP_SENATE_BELOW = 10,
    TOOLTIP_SENATE_ABOVE = 32,
    TOOLTIP_SENATE_ROWS = 6,
    TOOLTIP_SENATE_ROW_HEIGHT = 14,
    TOOLTIP_SENATE_VALUE_X = 140,
    TOOLTIP_SENATE_TEXT_GROUP = 68,
    TOOLTIP_NUMBER_PREFIX_NONE = 0x40,
    DISPLAY_MODE_RGB565 = 2,
    TOOLTIP_FILL_RGB565 = 0xbb88,
    TOOLTIP_FRAME_RGB565 = 0x58a0,
    TOOLTIP_SHADOW_RGB565 = 0x3922,
    TOOLTIP_TEXT_RGB565 = 0xff3a,
    TOOLTIP_FILL_RGB555 = 0x5dc8,
    TOOLTIP_FRAME_RGB555 = 0x2c40,
    TOOLTIP_SHADOW_RGB555 = 0x1c82,
    TOOLTIP_TEXT_RGB555 = 0x7f9a
};

static int32_t add32(int32_t left, int32_t right)
{
    return (int32_t)((uint32_t)left + (uint32_t)right);
}

static void box_colors(const PharaohTooltipGoodsDrawing *drawing,
    int32_t *frame, int32_t *fill)
{
    if (*drawing->display_mode == DISPLAY_MODE_RGB565) {
        *frame = TOOLTIP_FRAME_RGB565;
        *fill = TOOLTIP_FILL_RGB565;
    } else {
        *frame = TOOLTIP_FRAME_RGB555;
        *fill = TOOLTIP_FILL_RGB555;
    }
}

static void draw_box(const PharaohTooltipGoodsDrawing *drawing,
    int32_t width, int32_t height)
{
    int32_t frame;
    int32_t fill;
    box_colors(drawing, &frame, &fill);
    pharaoh_graphics_draw_rect(drawing->graphics, *drawing->box_x,
        *drawing->box_y, width, height, frame);
    pharaoh_graphics_fill_rect(drawing->graphics,
        add32(*drawing->box_x, 1), add32(*drawing->box_y, 1),
        add32(width, -2), add32(height, -2), fill);
}

static void measure_composed(const PharaohComposedTooltipContext *context,
    int32_t width)
{
    PharaohRichTextState *rich = context->rich_text;
    rich->scroll_position = 0;
    rich->viewport_lines = TOOLTIP_VIEWPORT_LINES;
    pharaoh_rich_text_draw(rich, context->rich_text_operations,
        context->text, add32(*context->drawing->box_x, TOOLTIP_PADDING),
        add32(*context->drawing->box_y, TOOLTIP_PADDING), width, 1, 0);
    *context->drawing->box_height =
        add32(rich->line_count * TOOLTIP_LINE_HEIGHT, TOOLTIP_EXTRA_HEIGHT);
}

void pharaoh_tooltip_draw_composed_text_box(
    const PharaohComposedTooltipContext *context)
{
    const PharaohTooltipGoodsDrawing *drawing = context->drawing;
    PharaohRichTextState *rich = context->rich_text;

    rich->normal_font = (uint32_t)*drawing->font_id;
    rich->link_font = (uint32_t)*drawing->font_id;
    *drawing->box_width = TOOLTIP_TEXT_WIDTH;
    *drawing->draw_color = TOOLTIP_MEASURE_COLOR;
    rich->color = TOOLTIP_MEASURE_COLOR;
    measure_composed(context, TOOLTIP_TEXT_INNER_WIDTH);
    if (rich->line_count > TOOLTIP_NARROW_MAX_LINES) {
        *drawing->box_width = TOOLTIP_TEXT_WIDE_WIDTH;
        measure_composed(context, TOOLTIP_TEXT_WIDE_INNER_WIDTH);
    }

    if (*drawing->mouse_x < add32(*drawing->box_width, TOOLTIP_BORDER))
        *drawing->box_x = add32(*drawing->mouse_x, TOOLTIP_MOUSE_GAP);
    else
        *drawing->box_x = add32(add32(*drawing->mouse_x,
            -*drawing->box_width), -TOOLTIP_MOUSE_GAP);
    *drawing->box_y = add32(*drawing->mouse_y, TOOLTIP_BELOW);
    if (*drawing->mouse_y >= TOOLTIP_LOWER_HALF_Y)
        *drawing->box_y = add32(*drawing->mouse_y, -TOOLTIP_ABOVE);

    draw_box(drawing, add32(*drawing->box_width, TOOLTIP_BORDER),
        *drawing->box_height);
    pharaoh_rich_text_draw(rich, context->rich_text_operations,
        context->text, add32(*drawing->box_x, TOOLTIP_PADDING),
        add32(*drawing->box_y, TOOLTIP_TEXT_TOP),
        add32(*drawing->box_width, -TOOLTIP_TEXT_INSET), 0, 0);
}

void pharaoh_tooltip_draw_senate(
    const PharaohSenateTooltipContext *context)
{
    static const int32_t labels[TOOLTIP_SENATE_ROWS] = {
        135, 136, 137, 138, 139, 142
    };
    const PharaohTooltipGoodsDrawing *drawing = context->drawing;
    const PharaohSenateTooltipValues *values = context->values;
    const int32_t row_values[TOOLTIP_SENATE_ROWS] = {
        values->unemployment, values->culture, values->prosperity,
        values->monument, values->kingdom, values->tax
    };
    const char *suffixes[TOOLTIP_SENATE_ROWS] = {
        context->percent_suffix, context->empty_suffix,
        context->empty_suffix, context->empty_suffix,
        context->empty_suffix, context->percent_suffix
    };
    const int32_t shadow = *drawing->display_mode == DISPLAY_MODE_RGB565 ?
        TOOLTIP_SHADOW_RGB565 : TOOLTIP_SHADOW_RGB555;
    const int32_t color = *drawing->display_mode == DISPLAY_MODE_RGB565 ?
        TOOLTIP_TEXT_RGB565 : TOOLTIP_TEXT_RGB555;
    int32_t row;
    int32_t y = 0;

    *drawing->box_width = TOOLTIP_SENATE_WIDTH;
    *drawing->box_height = TOOLTIP_SENATE_HEIGHT;
    *drawing->box_x = *drawing->mouse_x < TOOLTIP_SENATE_RIGHT_EDGE ?
        add32(*drawing->mouse_x, TOOLTIP_MOUSE_GAP) :
        add32(*drawing->mouse_x, -TOOLTIP_SENATE_RIGHT_EDGE);
    *drawing->box_y = add32(*drawing->mouse_y, TOOLTIP_SENATE_BELOW);
    if (*drawing->mouse_y >= TOOLTIP_LOWER_HALF_Y)
        *drawing->box_y = add32(*drawing->mouse_y, -TOOLTIP_SENATE_ABOVE);
    draw_box(drawing, TOOLTIP_SENATE_WIDTH, TOOLTIP_SENATE_HEIGHT);

    for (row = 0; row < TOOLTIP_SENATE_ROWS;
         ++row, y = add32(y, TOOLTIP_SENATE_ROW_HEIGHT)) {
        const int32_t text_x = add32(*drawing->box_x, TOOLTIP_PADDING);
        const int32_t value_x = add32(*drawing->box_x,
            TOOLTIP_SENATE_VALUE_X);
        const int32_t row_y = add32(add32(y, TOOLTIP_PADDING),
            *drawing->box_y);
        pharaoh_bitmap_text_draw_selection_native(drawing->font,
            drawing->text_state, drawing->text_selection,
            TOOLTIP_SENATE_TEXT_GROUP, labels[row], text_x, row_y,
            *drawing->font_id, (uint16_t)shadow);
        pharaoh_bitmap_text_draw_selection_native(drawing->font,
            drawing->text_state, drawing->text_selection,
            TOOLTIP_SENATE_TEXT_GROUP, labels[row], add32(text_x, -1),
            add32(row_y, -1), *drawing->font_id, (uint16_t)color);
        pharaoh_bitmap_text_draw_number_native(drawing->font,
            drawing->text_state, context->number_scratch, row_values[row],
            TOOLTIP_NUMBER_PREFIX_NONE, suffixes[row], value_x, row_y,
            *drawing->font_id, (uint16_t)shadow);
        pharaoh_bitmap_text_draw_number_native(drawing->font,
            drawing->text_state, context->number_scratch, row_values[row],
            TOOLTIP_NUMBER_PREFIX_NONE, suffixes[row], add32(value_x, -1),
            add32(row_y, -1), *drawing->font_id, (uint16_t)color);
    }
}
