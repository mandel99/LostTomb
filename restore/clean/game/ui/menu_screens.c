#include "pharaoh/ui/menu_screens.h"

#include <string.h>

enum {
    FULL_REDRAW = 1, PIXEL_BYTES = 2,
    GAME_TEXT_GROUP = 293, GAME_TITLE_ENTRY = 5, TITLE_CAPACITY = 200,
    PANEL_X = 128, PANEL_Y = 40, PANEL_WIDTH = 24, PANEL_HEIGHT = 21,
    TITLE_X = 140, TITLE_Y = 60, TITLE_WIDTH = 368,
    PROMPT_GROUP = 13, PROMPT_ENTRY = 7, PROMPT_WIDTH = 320,
    PROMPT_Y = 462, PROMPT_WHITE = 65535
};

/* Native clear-surface helper and its byte-fill child reduce to memset.
 * Native DWORD multiplication wraps before the byte count is passed. */
static void clear_screen(const PharaohMenuScreenState *s)
{
    if (s->screen_pixels != NULL) {
        uint32_t bytes = s->screen_height * s->screen_width * PIXEL_BYTES;
        memset(s->screen_pixels, 0, bytes);
    }
}

void pharaoh_main_menu_draw_background(const PharaohMenuScreenContext *c)
{
    PharaohMenuScreenState *s = c->screen;
    const PharaohMenuScreenOperations *ops = c->operations;
    const PharaohLabelRenderer *labels = c->renderer->labels;
    int32_t saved_expansion_control;
    char title[TITLE_CAPACITY];

    if (s->redraw_request == FULL_REDRAW) {
        clear_screen(s);
        ops->draw_sprite(ops->context, s->background_image,
            labels->origin_x, labels->origin_y);
    }
    ops->draw_panel(ops->context, labels->origin_x + PANEL_X,
        labels->origin_y + PANEL_Y, PANEL_WIDTH, PANEL_HEIGHT);
    pharaoh_text_select(c->text, GAME_TEXT_GROUP, GAME_TITLE_ENTRY);
    saved_expansion_control = s->text_expansion_control;
    s->text_expansion_control = 0;
    ops->expand_event_text(ops->context, c->text->current, title, TITLE_CAPACITY);
    s->text_expansion_control = saved_expansion_control;
    ops->draw_centered_text(ops->context, title, labels->origin_x + TITLE_X,
        labels->origin_y + TITLE_Y, TITLE_WIDTH, s->title_font, 0);
    pharaoh_main_menu_draw_foreground(c->renderer, c->menu, c->sound,
        c->game_menu_rects);
}

void pharaoh_logo_window_draw(const PharaohMenuScreenContext *c)
{
    const PharaohMenuScreenState *s = c->screen;
    const PharaohMenuScreenOperations *ops = c->operations;
    const PharaohLabelRenderer *labels = c->renderer->labels;

    if (c->menu->digital_sound_ok &&
        !c->sound->is_playing(c->sound->context, 0) && c->menu->music_enabled)
        c->sound->play(c->sound->context, "audio\\music\\setup.mp3", 0, 0);
    clear_screen(s);
    ops->draw_sprite(ops->context, s->logo_image, labels->origin_x, labels->origin_y);
    ops->draw_centered_language_ui(ops->context, PROMPT_GROUP, PROMPT_ENTRY,
        (s->window_width - PROMPT_WIDTH) / 2, labels->origin_y + PROMPT_Y,
        PROMPT_WIDTH, s->prompt_font, PROMPT_WHITE, labels->origin_x, labels->origin_y);
}
