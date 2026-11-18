#include "pharaoh/ui/menu_button_frame.h"

enum { BUTTON_FRAME_TILE_SIZE = 16, BUTTON_FRAME_SELECTED_OFFSET = 8 };

static int32_t tile_count(int32_t pixels)
{
    return pixels / BUTTON_FRAME_TILE_SIZE +
        (pixels % BUTTON_FRAME_TILE_SIZE != 0);
}

void pharaoh_menu_button_frame_draw(
    const PharaohMenuButtonFrameOperations *operations, int32_t base_image,
    int32_t x, int32_t y, int32_t width, int32_t height, int32_t selected)
{
    int32_t columns;
    int32_t rows;
    int32_t right_crop;
    int32_t bottom_crop;

    if (operations == NULL || operations->draw_sprite == NULL ||
        width <= 0 || height <= 0) {
        return;
    }
    if (selected != 0) base_image += BUTTON_FRAME_SELECTED_OFFSET;
    columns = tile_count(width);
    rows = tile_count(height);
    right_crop = columns * BUTTON_FRAME_TILE_SIZE - width;
    bottom_crop = rows * BUTTON_FRAME_TILE_SIZE - height;

    for (int32_t row = 0; row < rows; ++row) {
        for (int32_t column = 0; column < columns; ++column) {
            int32_t image;
            int32_t image_x = x + column * BUTTON_FRAME_TILE_SIZE;
            int32_t image_y = y + row * BUTTON_FRAME_TILE_SIZE;

            if (row == 0) {
                if (column == 0) image = base_image;
                else if (column < columns - 1) image = base_image + 1;
                else {
                    image = base_image + 2;
                    image_x -= right_crop;
                }
            } else if (row < rows - 1) {
                if (column == 0) image = base_image + 7;
                else if (column == columns - 1) {
                    image = base_image + 3;
                    image_x -= right_crop;
                } else {
                    continue;
                }
            } else {
                image_y -= bottom_crop;
                if (column == 0) image = base_image + 6;
                else if (column < columns - 1) image = base_image + 5;
                else {
                    image = base_image + 4;
                    image_x -= right_crop;
                }
            }
            operations->draw_sprite(operations->user_data, image,
                image_x, image_y);
        }
    }
}
