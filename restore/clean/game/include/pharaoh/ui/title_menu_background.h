#ifndef PHARAOH_UI_TITLE_MENU_BACKGROUND_H
#define PHARAOH_UI_TITLE_MENU_BACKGROUND_H

#include <stdint.h>
#include "pharaoh/runtime/startup_asset_loading.h"

typedef struct PharaohTitleMenuBackgroundContext {
    PharaohStartupAssetState *asset_state;
    const PharaohStartupAssetOperations *operations;
    uint32_t screen_width;
    uint32_t screen_height;
    uint32_t background_image;
    int32_t image_x;
    int32_t image_y;
} PharaohTitleMenuBackgroundContext;

typedef struct PharaohTitleMenuRedrawState {
    PharaohTitleMenuBackgroundContext *background;
    uint8_t *redraw_requested;
} PharaohTitleMenuRedrawState;

/* Native entry 0x00520AB0, used by the screen dispatcher for window 0. */
void pharaoh_title_menu_background_draw(
    const PharaohTitleMenuBackgroundContext *context,
    int32_t skip_surface_clear);

/* The window-0 branch extracted from native redraw dispatcher 0x004D71C0.
   The caller resolves the native screen-selection override before entry. */
int32_t pharaoh_title_menu_redraw_window_zero(
    PharaohTitleMenuRedrawState *state, int32_t force_redraw);

#endif
