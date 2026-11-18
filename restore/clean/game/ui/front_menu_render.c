#include "pharaoh/ui/front_menu_render.h"

enum {
    FRONT_MENU_MUSIC_CHANNEL = 0,
    FRONT_MENU_MUSIC_FLAGS = 0
};

void pharaoh_front_menu_update_music(int32_t sound_output_ready,
    int32_t music_enabled, const PharaohMainMenuSound *sound)
{
    if (sound_output_ready != 0 &&
        sound->is_playing(sound->context,
            FRONT_MENU_MUSIC_CHANNEL) == 0 &&
        music_enabled != 0) {
        sound->play(sound->context,
            "audio\\music\\setup.mp3", FRONT_MENU_MUSIC_CHANNEL,
            FRONT_MENU_MUSIC_FLAGS);
    }
}

void pharaoh_front_menu_render_window_zero(
    const PharaohFrontMenuRenderContext *context)
{
    pharaoh_front_menu_update_music(context->sound_output_ready,
        context->music_enabled, context->sound);

    /* Native 0x0051F780 is a no-op; the next effect is the 0x004180C0 draw. */
    pharaoh_front_menu_draw(context->renderer, context->menu_state,
        context->button_rectangles, context->button_labels);
}
