#include "pharaoh/ui/panel_frame.h"

void pharaoh_panel_frame_draw(const PharaohPanelFrameOperations *operations,
    int32_t x, int32_t y, int32_t width_blocks, int32_t height_blocks)
{
    int32_t vertical_offset = 0;
    int32_t vertical_step = 0;

    for (int32_t row = 0; row < height_blocks; ++row) {
        int32_t column_offset = 0;
        for (int32_t column = 0; column < width_blocks; ++column) {
            int32_t image;
            if (row == 0) {
                if (column == 0) {
                    image = operations->panel_image_base;
                } else if (column < width_blocks - 1) {
                    image = operations->panel_image_base + 1 + column_offset;
                    ++column_offset;
                } else {
                    image = operations->panel_image_base + 0x0b;
                }
                vertical_step = 0;
            } else if (row < height_blocks - 1) {
                if (column == 0) {
                    image = operations->panel_image_base + 0x0c +
                        vertical_offset;
                } else if (column < width_blocks - 1) {
                    image = operations->panel_image_base + column_offset +
                        0x0d + vertical_offset;
                    ++column_offset;
                } else {
                    image = operations->panel_image_base + 0x17 +
                        vertical_offset;
                }
                vertical_step = 0x0c;
            } else if (column == 0) {
                image = operations->panel_image_base + 0x84;
                vertical_step = 0;
            } else if (column < width_blocks - 1) {
                image = operations->panel_image_base + 0x85 + column_offset;
                ++column_offset;
                vertical_step = 0;
            } else {
                image = operations->panel_image_base + 0x8f;
                vertical_step = 0;
            }

            operations->draw_sprite(operations->user_data, image,
                (int32_t)((uint32_t)x + (uint32_t)column * 16u),
                (int32_t)((uint32_t)y + (uint32_t)row * 16u));
            if (column_offset > 9) column_offset = 0;
        }
        vertical_offset += vertical_step;
        if (vertical_offset > 0x77) vertical_offset = 0;
    }
}

void pharaoh_inner_panel_frame_draw(
    const PharaohPanelFrameOperations *operations,
    int32_t x, int32_t y, int32_t width_blocks, int32_t height_blocks)
{
    int32_t vertical_offset = 0;
    int32_t vertical_step = 0;

    for (int32_t row = 0; row < height_blocks; ++row) {
        int32_t column_offset = 0;
        for (int32_t column = 0; column < width_blocks; ++column) {
            int32_t image;
            if (row == 0) {
                if (column == 0) {
                    image = operations->inner_panel_image_base;
                } else if (column < width_blocks - 1) {
                    image = operations->inner_panel_image_base + 1 +
                        column_offset;
                    ++column_offset;
                } else {
                    image = operations->inner_panel_image_base + 6;
                }
                vertical_step = 0;
            } else if (row < height_blocks - 1) {
                if (column == 0) {
                    image = operations->inner_panel_image_base + 7 +
                        vertical_offset;
                } else if (column < width_blocks - 1) {
                    image = operations->inner_panel_image_base + column_offset +
                        8 + vertical_offset;
                    ++column_offset;
                } else {
                    image = operations->inner_panel_image_base + 0x0d +
                        vertical_offset;
                }
                vertical_step = 7;
            } else if (column == 0) {
                image = operations->inner_panel_image_base + 0x2a;
                vertical_step = 0;
            } else if (column < width_blocks - 1) {
                image = operations->inner_panel_image_base + 0x2b +
                    column_offset;
                ++column_offset;
                vertical_step = 0;
            } else {
                image = operations->inner_panel_image_base + 0x30;
                vertical_step = 0;
            }

            operations->draw_sprite(operations->user_data, image,
                (int32_t)((uint32_t)x + (uint32_t)column * 16u),
                (int32_t)((uint32_t)y + (uint32_t)row * 16u));
            if (column_offset > 4) column_offset = 0;
        }
        vertical_offset += vertical_step;
        if (vertical_offset > 0x22) vertical_offset = 0;
    }
}

static void native_frame_image(void *data, int32_t offset, int32_t x, int32_t y)
{
    const PharaohPanelFrameNativeContext *const context = data;
    *context->tile_x_00d3b5e0 = x;
    *context->tile_y_00d3b5dc = y;
    const int32_t image = (int32_t)((uint32_t)*context->image_base +
        (uint32_t)offset);
    *context->current_image_00d3b5c0 = (uint32_t)image;
    (void)pharaoh_image_draw(context->drawing, image, x, y);
}

void pharaoh_panel_frame_draw_native(const PharaohPanelFrameNativeContext *context,
    int32_t x, int32_t y, int32_t width_blocks, int32_t height_blocks)
{
    /* Zero selects relative sprite offsets in the shared pattern core.
       The provider reads the actual live native image base on every tile. */
    const PharaohPanelFrameOperations offsets = {
        (void *)context, native_frame_image, 0, 0
    };
    pharaoh_panel_frame_draw(&offsets, x, y, width_blocks, height_blocks);
}

void pharaoh_inner_panel_frame_draw_native(const PharaohPanelFrameNativeContext *context,
    int32_t x, int32_t y, int32_t width_blocks, int32_t height_blocks)
{
    const PharaohPanelFrameOperations offsets = {
        (void *)context, native_frame_image, 0, 0
    };
    pharaoh_inner_panel_frame_draw(&offsets, x, y, width_blocks, height_blocks);
}
