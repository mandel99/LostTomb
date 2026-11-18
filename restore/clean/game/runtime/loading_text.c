#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "pharaoh/runtime/loading_text.h"
#include "pharaoh/core/text_length.h"

enum {
    LOADING_TEXT_OUTER_X_MARGIN = 26,
    LOADING_TEXT_OUTER_Y_MARGIN = 11,
    LOADING_TEXT_INNER_X_MARGIN = 25,
    LOADING_TEXT_INNER_Y_MARGIN = 10,
    LOADING_TEXT_DESKTOP_Y_ADJUSTMENT = 24,
    LOADING_TEXT_VERTICAL_OFFSET_PERCENT = 50,
    LOADING_TEXT_PERCENT_DIVISOR = 100,
    LOADING_TEXT_BACKGROUND_COLOR = 0x00ffffff
};

/* 004cf7b0 draws the measured text over two nested native fill rectangles. */
void pharaoh_loading_text_draw(const char *text,
    const PharaohGraphicsStartupState *graphics,
    PharaohGraphicsPrimitives *primitives)
{
    const int32_t character_count = pharaoh_text_length(text);
    HDC device_context;
    SIZE text_extent;

    if (character_count <= 0) {
        return;
    }

    device_context = GetDC(NULL);
    if (device_context == NULL) {
        return;
    }

    (void)GetTextExtentPointA(device_context, text, character_count,
        &text_extent);

    const int32_t text_center_x =
        (graphics->screen_width - text_extent.cx) / 2;
    const int32_t half_screen_y =
        (graphics->screen_height - text_extent.cy) / 2;
    const int32_t vertical_text_offset =
        (half_screen_y * LOADING_TEXT_VERTICAL_OFFSET_PERCENT) /
            LOADING_TEXT_PERCENT_DIVISOR;
    int32_t text_top = half_screen_y + vertical_text_offset;
    const int32_t desktop_text_x =
        (graphics->desktop_width - text_extent.cx) / 2;
    const int32_t desktop_text_y =
        (graphics->desktop_height - text_extent.cy) / 2 +
            vertical_text_offset;

    if (graphics->desktop_height != graphics->screen_height) {
        text_top -= LOADING_TEXT_DESKTOP_Y_ADJUSTMENT;
    }

    pharaoh_graphics_fill_rect(primitives,
        text_center_x - LOADING_TEXT_OUTER_X_MARGIN,
        text_top - LOADING_TEXT_OUTER_Y_MARGIN,
        text_extent.cx + 2 * LOADING_TEXT_OUTER_X_MARGIN,
        text_extent.cy + 2 * LOADING_TEXT_OUTER_Y_MARGIN, 0);
    pharaoh_graphics_fill_rect(primitives,
        text_center_x - LOADING_TEXT_INNER_X_MARGIN,
        text_top - LOADING_TEXT_INNER_Y_MARGIN,
        text_extent.cx + 2 * LOADING_TEXT_INNER_X_MARGIN,
        text_extent.cy + 2 * LOADING_TEXT_INNER_Y_MARGIN, 0xffff);
    (void)SetBkColor(device_context, LOADING_TEXT_BACKGROUND_COLOR);
    (void)TextOutA(device_context, desktop_text_x, desktop_text_y,
        text, character_count);
    (void)ReleaseDC(NULL, device_context);
}
