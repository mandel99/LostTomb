#include "pharaoh/ui/button_border.h"

static int32_t add(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a + (uint32_t)b); }
static int32_t sub(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a - (uint32_t)b); }
static int32_t times16(int32_t value)
{ return (int32_t)((uint32_t)value * 16u); }

void pharaoh_button_border_draw(const PharaohButtonBorderContext *context,
    int32_t style, int32_t x, int32_t y, int32_t width, int32_t height,
    uint32_t flags)
{
    const int32_t columns = width / 16 + (width % 16 != 0);
    const int32_t rows = height / 16 + (height % 16 != 0);
    const int32_t right_trim = sub(times16(columns), width);
    const int32_t bottom_trim = sub(times16(rows), height);
    const int32_t base = add(*context->image_base, flags != 0 ? 8 : 0);
    (void)style;
    for (int32_t row = 0; row < rows; ++row) {
        const int32_t row_y = add(times16(row), y);
        for (int32_t column = 0; column < columns; ++column) {
            *context->tile_x_00d3b5e0 = add(times16(column), x);
            *context->tile_y_00d3b5dc = row_y;
            int32_t draw_x = *context->tile_x_00d3b5e0;
            int32_t draw_y = row_y;
            int32_t image;
            if (row == 0) {
                if (column == 0) image = base;
                else if (column < columns - 1) image = add(base, 1);
                else { image = add(base, 2); draw_x = sub(draw_x, right_trim); }
            } else if (row >= rows - 1) {
                draw_y = sub(draw_y, bottom_trim);
                if (column == 0) image = add(base, 6);
                else if (column < columns - 1) image = add(base, 5);
                else { image = add(base, 4); draw_x = sub(draw_x, right_trim); }
            } else if (column == 0) {
                image = add(base, 7);
            } else if (column >= columns - 1) {
                image = add(base, 3);
                draw_x = sub(draw_x, right_trim);
            } else {
                continue;
            }
            (void)pharaoh_image_draw(context->drawing, image, draw_x, draw_y);
        }
    }
}

void pharaoh_ui_layout_clear_highlight(const PharaohPanelFrameNativeContext *panel)
{ pharaoh_panel_frame_draw_native(panel, 0, 0, 6, 6); }

static void clear(void *data)
{
    PharaohLayoutHighlightDrawing *const drawing = data;
    pharaoh_ui_layout_clear_highlight(&drawing->panel);
}
static void border(void *data, int32_t style, int32_t x, int32_t y,
    int32_t width, int32_t height, uint32_t flags)
{
    PharaohLayoutHighlightDrawing *const drawing = data;
    pharaoh_button_border_draw(&drawing->border,
        style, x, y, width, height, flags);
}

void pharaoh_ui_layout_bind_highlight(PharaohLayoutHighlightState *state,
    PharaohLayoutHighlightDrawing *drawing, PharaohImageDrawContext *images,
    const int32_t *panel_image_base, const int32_t *border_image_base,
    uint32_t *current_image, int32_t *tile_x, int32_t *tile_y)
{
    drawing->panel = (PharaohPanelFrameNativeContext){images, panel_image_base,
        current_image, tile_x, tile_y};
    drawing->border = (PharaohButtonBorderContext){images, border_image_base,
        tile_x, tile_y};
    state->user_data = drawing;
    state->clear_highlight = clear;
    state->draw_border = border;
}
