#ifndef PHARAOH_UI_FRONT_MENU_RENDER_H
#define PHARAOH_UI_FRONT_MENU_RENDER_H

#include "pharaoh/ui/main_menu.h"

typedef struct PharaohFrontMenuRenderContext {
    int32_t sound_output_ready;
    int32_t music_enabled;
    const PharaohMenuRenderer *renderer;
    const PharaohMainMenuState *menu_state;
    const PharaohMainMenuSound *sound;
    const PharaohMenuRect *button_rectangles;
    const int32_t *button_labels;
} PharaohFrontMenuRenderContext;

/* The window-zero foreground branch of native 0x004D75D0. */
void pharaoh_front_menu_update_music(int32_t sound_output_ready,
    int32_t music_enabled, const PharaohMainMenuSound *sound);

void pharaoh_front_menu_render_window_zero(
    const PharaohFrontMenuRenderContext *context);

#endif
