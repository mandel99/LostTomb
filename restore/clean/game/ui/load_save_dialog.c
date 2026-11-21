#include "pharaoh/ui/load_save_dialog.h"

enum { LOAD_ROWS = 12, ALTERNATE_ROWS = 15 };

int32_t pharaoh_load_dialog_scroll(int32_t direction, int32_t file_count,
    int32_t *first_visible, uint8_t *error_countdown)
{
    if (file_count <= LOAD_ROWS) return 0;
    if (direction == 0) {
        if (*first_visible <= 0) return 0;
        --*first_visible;
    } else {
        if (direction != 1 || *first_visible + LOAD_ROWS >= file_count) {
            if (direction != 1) *error_countdown = 0;
            return 0;
        }
        ++*first_visible;
    }
    *error_countdown = 0;
    return 1;
}

int32_t pharaoh_load_dialog_drag_scrollbar(int32_t press_active,
    int32_t mouse_x, int32_t mouse_y, int32_t origin_x, int32_t origin_y,
    int32_t file_count, int32_t *first_visible)
{
    int32_t position;
    int32_t percentage;
    if (file_count < 13 || press_active == 0 ||
        mouse_x < origin_x + 0x1d0 || mouse_x > origin_x + 0x1f0 ||
        mouse_y < origin_y + 0x91 || mouse_y > origin_y + 300) {
        return 0;
    }
    position = mouse_y - origin_y - 0x91;
    if (position > 0x82) position = 0x82;
    percentage = position * 100 / 0x82;
    *first_visible = (file_count - LOAD_ROWS) * percentage / 100;
    return 1;
}

int32_t pharaoh_load_dialog_select_visible(const PharaohFileList *files,
    int32_t first_visible, int32_t visible_row)
{
    const int32_t index = first_visible + visible_row;
    return visible_row >= 0 && visible_row < LOAD_ROWS &&
        pharaoh_file_list_name(files, index) != NULL ? index : -1;
}

int32_t pharaoh_file_dialog_scroll_thumb(int32_t alternate_mode,
    int32_t file_count, int32_t first_visible, int32_t *x, int32_t *y)
{
    const int32_t rows = alternate_mode != 0 ? ALTERNATE_ROWS : LOAD_ROWS;
    const int32_t range = alternate_mode != 0 ? 0xa4 : 0x82;
    int32_t percentage;
    if (file_count <= rows) return 0;
    if (first_visible < 1) percentage = 0;
    else if (first_visible + rows >= file_count) percentage = 100;
    else percentage = first_visible * 100 / (file_count - rows);
    *x = alternate_mode != 0 ? 0x11c : 0x1d8;
    *y = (alternate_mode != 0 ? 0xf5 : 0x91) +
        range * percentage / 100;
    return 1;
}
