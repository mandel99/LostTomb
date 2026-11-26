#include "pharaoh/ui/panel_blocks.h"

static int32_t add(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a + (uint32_t)b); }
static int32_t sub(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a - (uint32_t)b); }
static int32_t mul(int32_t a, uint32_t b)
{ return (int32_t)((uint32_t)a * b); }
static int32_t part(int32_t index, int32_t count)
{ return index == 0 ? 0 : index < count - 1 ? 1 : 2; }

void pharaoh_panel_blocks_draw(const PharaohPanelBlocksContext *c, int32_t kind,
    int32_t x, int32_t y, int32_t width, int32_t height, int32_t style)
{
    const int32_t value = c->animations[kind];
    if (value <= 4) return;
    if (value < 15) {
        const PharaohPanelOpeningFrame *const frame = &c->opening_frames[value - 5];
        *c->draw_x = add(add(mul(sub(width, 2), 16u) / 2, frame->x), x);
        *c->draw_y = add(add(mul(sub(height, 2), 16u) / 2, frame->y), y);
        const int32_t image = add(add(add(frame->image, 24), mul(style, 8u)),
            *c->image_base);
        (void)pharaoh_image_draw(c->drawing, image, *c->draw_x, *c->draw_y);
        return;
    }
    const int32_t blocks = sub(value, 13);
    int32_t columns;
    if (width < blocks) columns = width;
    else { x = add(x, mul(sub(width, blocks), 16u) / 2); columns = blocks; }
    if (blocks <= height) { y = add(y, mul(sub(height, blocks), 16u) / 2); height = blocks; }
    for (int32_t row = 0; row < height; ++row) {
        const int32_t row_y = add(mul(row, 16u), y);
        for (int32_t column = 0; column < columns; ++column) {
            *c->draw_x = add(mul(column, 16u), x);
            const int32_t offset = add(add(mul(style, 9u), 6),
                add(mul(part(row, height), 3u), part(column, columns)));
            *c->draw_y = row_y;
            *c->current_image = (uint32_t)add(*c->image_base, offset);
            if (kind == 8)
                (void)pharaoh_image_draw_masked(c->drawing,
                    (int32_t)*c->current_image, *c->draw_x, row_y, UINT32_C(0xfffff863));
            else (void)pharaoh_image_draw(c->drawing,
                (int32_t)*c->current_image, *c->draw_x, row_y);
        }
    }
}
