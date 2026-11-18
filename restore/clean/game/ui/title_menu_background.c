#include "pharaoh/ui/title_menu_background.h"

#include <string.h>

enum {
    TITLE_MENU_BACKGROUND_PROGRESS_VALUE = 100,
    TITLE_MENU_PIXEL_BYTES_PER_PIXEL = 2
};

void pharaoh_title_menu_background_draw(
    const PharaohTitleMenuBackgroundContext *context,
    int32_t skip_surface_clear)
{
    if (skip_surface_clear == 0 &&
        context->asset_state->screen_pixel_buffer != NULL) {
        const uint32_t screen_pixel_byte_count = context->screen_width *
            context->screen_height * TITLE_MENU_PIXEL_BYTES_PER_PIXEL;
        memset(context->asset_state->screen_pixel_buffer, 0,
            screen_pixel_byte_count);
    }

    context->operations->draw_sprite(context->operations->user_data,
        context->background_image, context->image_x, context->image_y);
    context->asset_state->loading_progress_percent =
        TITLE_MENU_BACKGROUND_PROGRESS_VALUE;
}

int32_t pharaoh_title_menu_redraw_window_zero(
    PharaohTitleMenuRedrawState *state, int32_t force_redraw)
{
    if (force_redraw == 0 && *state->redraw_requested == 0) {
        return 0;
    }

    if (force_redraw != 0) {
        *state->redraw_requested = 1;
    }

    pharaoh_title_menu_background_draw(state->background, 0);
    *state->redraw_requested = 0;
    return 1;
}
