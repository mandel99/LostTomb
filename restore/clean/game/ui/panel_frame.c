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
                x + column * 16, y + row * 16);
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
                x + column * 16, y + row * 16);
            if (column_offset > 4) column_offset = 0;
        }
        vertical_offset += vertical_step;
        if (vertical_offset > 0x22) vertical_offset = 0;
    }
}
