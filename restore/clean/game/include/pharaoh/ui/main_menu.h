#ifndef PHARAOH_UI_MAIN_MENU_H
#define PHARAOH_UI_MAIN_MENU_H

#include "pharaoh/ui/label_text.h"

enum { PHARAOH_MENU_BUTTON_COUNT = 5 };

typedef struct PharaohMenuRect {
    int16_t left, top, right, bottom;
} PharaohMenuRect;

/* Only the geometry consumed by drawing is projected from GenericButton.
 * Input callbacks and button parameters belong to the input owner. */
extern const PharaohMenuRect pharaoh_front_menu_rects[PHARAOH_MENU_BUTTON_COUNT];
extern const PharaohMenuRect pharaoh_game_menu_rects[PHARAOH_MENU_BUTTON_COUNT];
extern const int32_t pharaoh_front_menu_labels[PHARAOH_MENU_BUTTON_COUNT];

typedef struct PharaohMenuRenderer {
    const PharaohLabelRenderer *labels;
    void *context;
    /* Native centered language text and button border raster endpoints. */
    void (*draw_centered)(void *, int32_t group, int32_t text,
        int32_t x, int32_t y, int32_t width, int32_t font, int32_t color);
    void (*draw_border)(void *, int32_t style, int32_t x, int32_t y,
        int32_t width, int32_t height, int32_t selected);
} PharaohMenuRenderer;

typedef struct PharaohMainMenuState {
    int32_t focused_button; /* Native one-based focus index. */
    int32_t font;
    int32_t digital_sound_ok;
    uint8_t music_enabled, can_start, has_saves, can_continue;
} PharaohMainMenuState;

typedef struct PharaohMainMenuSound {
    void *context;
    void (*update)(void *, int32_t mode);
    int32_t (*is_playing)(void *, int32_t channel);
    void (*play)(void *, const char *path, int32_t channel, int32_t flags);
} PharaohMainMenuSound;

void pharaoh_menu_draw_centered_rect(const PharaohMenuRenderer *renderer,
    int32_t group, int32_t text, const PharaohMenuRect *rect,
    int32_t font, int32_t color, int32_t dx, int32_t dy);
void pharaoh_menu_draw_border_rect(const PharaohMenuRenderer *renderer,
    int32_t style, const PharaohMenuRect *rect, int32_t selected,
    int32_t dx, int32_t dy);
void pharaoh_front_menu_draw(const PharaohMenuRenderer *renderer,
    const PharaohMainMenuState *state,
    const PharaohMenuRect rects[PHARAOH_MENU_BUTTON_COUNT],
    const int32_t labels[PHARAOH_MENU_BUTTON_COUNT]);
void pharaoh_main_menu_draw_foreground(const PharaohMenuRenderer *renderer,
    const PharaohMainMenuState *state, const PharaohMainMenuSound *sound,
    const PharaohMenuRect rects[PHARAOH_MENU_BUTTON_COUNT]);

#endif
