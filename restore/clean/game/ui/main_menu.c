#include "pharaoh/ui/main_menu.h"

enum {
    FRONT_TEXT_GROUP = 30, GAME_TEXT_GROUP = 293, START_TEXT = 7,
    FRONT_ANIMATION = 6, LABEL_BLOCK_WIDTH = 16,
    FRONT_TEXT_Y = 6, GAME_TEXT_X = 4, GAME_TEXT_Y = 7,
    MENU_COLOR = 0, MENU_BORDER_STYLE = 0
};

/* Geometry and label IDs from the pinned executable's initialized tables.
 * These are data values, never addresses into the original image. */
const PharaohMenuRect pharaoh_front_menu_rects[PHARAOH_MENU_BUTTON_COUNT] = {
    {192, 100, 448, 125}, {192, 140, 448, 165}, {192, 180, 448, 205},
    {192, 220, 448, 245}, {192, 260, 448, 285}
};
const int32_t pharaoh_front_menu_labels[PHARAOH_MENU_BUTTON_COUNT] = {0, 2, 3, 5, 4};
const PharaohMenuRect pharaoh_game_menu_rects[PHARAOH_MENU_BUTTON_COUNT] = {
    {208, 128, 433, 153}, {208, 176, 433, 201}, {208, 224, 433, 249},
    {208, 272, 433, 297}, {208, 320, 433, 345}
};

void pharaoh_menu_draw_centered_rect(const PharaohMenuRenderer *r,
    int32_t group, int32_t text, const PharaohMenuRect *rect,
    int32_t font, int32_t color, int32_t dx, int32_t dy)
{
    r->draw_centered(r->context, group, text,
        rect->left + r->labels->origin_x + dx,
        rect->top + r->labels->origin_y + dy,
        (int32_t)rect->right - rect->left, font, color);
}

void pharaoh_menu_draw_border_rect(const PharaohMenuRenderer *r,
    int32_t style, const PharaohMenuRect *rect, int32_t selected,
    int32_t dx, int32_t dy)
{
    r->draw_border(r->context, style,
        rect->left + r->labels->origin_x + dx,
        rect->top + r->labels->origin_y + dy,
        (int32_t)rect->right - rect->left,
        (int32_t)rect->bottom - rect->top, selected);
}

void pharaoh_front_menu_draw(const PharaohMenuRenderer *r,
    const PharaohMainMenuState *s,
    const PharaohMenuRect rects[PHARAOH_MENU_BUTTON_COUNT],
    const int32_t labels[PHARAOH_MENU_BUTTON_COUNT])
{
    int32_t i;
    for (i = 0; i < PHARAOH_MENU_BUTTON_COUNT; ++i) {
        const PharaohMenuRect *rect = &rects[i];
        pharaoh_large_label_draw_animated(r->labels, FRONT_ANIMATION,
            rect->left + r->labels->origin_x, rect->top + r->labels->origin_y,
            ((int32_t)rect->right - rect->left) / LABEL_BLOCK_WIDTH,
            s->focused_button == i + 1);
        pharaoh_menu_draw_centered_rect(r, FRONT_TEXT_GROUP, labels[i], rect,
            s->font, MENU_COLOR, 0, FRONT_TEXT_Y);
    }
}

void pharaoh_main_menu_draw_foreground(const PharaohMenuRenderer *r,
    const PharaohMainMenuState *s, const PharaohMainMenuSound *sound,
    const PharaohMenuRect rects[PHARAOH_MENU_BUTTON_COUNT])
{
    int32_t i, font;
    sound->update(sound->context, 1);
    /* Preserve the native short-circuit order, including the playing query. */
    if (s->digital_sound_ok && !sound->is_playing(sound->context, 0) &&
        s->music_enabled)
        sound->play(sound->context, "audio\\music\\setup.mp3", 0, 0);
    font = s->font;
    for (i = 0; i < PHARAOH_MENU_BUTTON_COUNT; ++i) {
        int32_t selected, text;
        if ((i == 1 && !s->can_start) || (i == 2 && !s->has_saves)) continue;
        selected = s->focused_button == i + 1;
        text = i == 0 && !s->can_continue ? START_TEXT : i;
        pharaoh_menu_draw_centered_rect(r, GAME_TEXT_GROUP, text, &rects[i],
            font, MENU_COLOR, GAME_TEXT_X, GAME_TEXT_Y);
        pharaoh_menu_draw_border_rect(r, MENU_BORDER_STYLE, &rects[i], selected, 0, 0);
    }
}
