#ifndef PHARAOH_UI_MENU_SCREENS_H
#define PHARAOH_UI_MENU_SCREENS_H

#include "pharaoh/ui/main_menu.h"
#include "pharaoh/runtime/text_loading.h"

typedef struct PharaohMenuScreenState {
    uint16_t *screen_pixels;
    uint32_t screen_width, screen_height;
    int32_t window_width;
    uint8_t redraw_request;
    int32_t background_image, logo_image, title_font, prompt_font;
    /* Shared with EventTextExpand; its wider meaning is not assumed here. */
    int32_t text_expansion_control;
} PharaohMenuScreenState;

typedef struct PharaohMenuScreenOperations {
    void *context;
    void (*draw_sprite)(void *, int32_t image, int32_t x, int32_t y);
    void (*draw_panel)(void *, int32_t x, int32_t y,
        int32_t width_blocks, int32_t height_blocks);
    void (*expand_event_text)(void *, const char *source, char *destination,
        int32_t capacity);
    void (*draw_centered_text)(void *, const char *, int32_t x, int32_t y,
        int32_t width, int32_t font, int32_t color);
    /* Layout-aware native language draw: origin arguments are significant. */
    void (*draw_centered_language_ui)(void *, int32_t group, int32_t entry,
        int32_t x, int32_t y, int32_t width, int32_t font, int32_t color,
        int32_t origin_x, int32_t origin_y);
} PharaohMenuScreenOperations;

typedef struct PharaohMenuScreenContext {
    PharaohMenuScreenState *screen;
    PharaohTextSelection *text;
    const PharaohMenuRenderer *renderer;
    const PharaohMainMenuState *menu;
    const PharaohMainMenuSound *sound;
    const PharaohMenuScreenOperations *operations;
    const PharaohMenuRect *game_menu_rects;
} PharaohMenuScreenContext;

void pharaoh_main_menu_draw_background(const PharaohMenuScreenContext *context);
void pharaoh_logo_window_draw(const PharaohMenuScreenContext *context);

#endif
