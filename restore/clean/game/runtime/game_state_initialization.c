#include "pharaoh/runtime/game_state_initialization.h"

enum {
    PHARAOH_STARTUP_FIELD_1_MAX_LENGTH = 31,
    PHARAOH_STARTUP_FIELD_1_WIDTH = 104,
    PHARAOH_STARTUP_DIALOG_MAX_LENGTH = 32,
    PHARAOH_STARTUP_DIALOG_WIDTH = 280,
    PHARAOH_STARTUP_FIELD_5_MAX_LENGTH = 24,
    PHARAOH_STARTUP_FIELD_5_WIDTH = 280,
    PHARAOH_STARTUP_FIELD_6_MAX_LENGTH = 500,
    PHARAOH_STARTUP_FIELD_6_WIDTH = 400,
    PHARAOH_STARTUP_CHEAT_MAX_LENGTH = 256,
    PHARAOH_STARTUP_CHEAT_WIDTH = 280
};

void pharaoh_game_state_initialize(const PharaohGameStateInitContext *c)
{
    PharaohGameStateInitState *const state = c->state;

    state->startup_state_byte = 0;
    state->startup_state_word = 0;
    state->scenario_climate = 0;
    state->debug_evolve_info_enabled = 0;
    state->debug_state_word = 0;
    state->debug_walker_info_enabled = 0;
    pharaoh_map_ring_initialize(c->map_ring);
    state->city_view_orientation = 0;
    pharaoh_city_view_calculate_lookup(c->city_view, 0);

    if (c->viewport_mode == 1) {
        pharaoh_city_view_set_viewport_without_sidebar(c->city_view);
    } else {
        pharaoh_city_view_set_viewport_with_sidebar(c->city_view);
    }

    c->city_view->state->camera_x = 109;
    c->city_view->state->camera_y = 218;
    state->map_view_orientation = 0;
    pharaoh_city_view_calculate_map_bounds(c->city_view);
    pharaoh_city_view_check_camera_boundaries(c->city_view);
    pharaoh_random_generator_fill_pool(c->random_generator);
    c->text_input_state->active_field = 0;

    pharaoh_text_input_field_initialize(c->text_input_state,
        PHARAOH_STARTUP_TEXT_FIELD_SCENARIO_NAME,
        c->text_buffers->field_1_buffer,
        PHARAOH_STARTUP_FIELD_1_MAX_LENGTH,
        PHARAOH_STARTUP_FIELD_1_WIDTH, 1,
        c->text_format_primary);
    pharaoh_text_input_field_initialize(c->text_input_state,
        PHARAOH_STARTUP_TEXT_FIELD_FILE_DIALOG_NAME,
        c->text_buffers->file_dialog_name, PHARAOH_STARTUP_DIALOG_MAX_LENGTH,
        PHARAOH_STARTUP_DIALOG_WIDTH, 1, c->text_format_secondary);
    pharaoh_text_input_field_initialize(c->text_input_state,
        PHARAOH_STARTUP_TEXT_FIELD_MAP_NAME,
        c->text_buffers->scenario_map_name, PHARAOH_STARTUP_DIALOG_MAX_LENGTH,
        PHARAOH_STARTUP_DIALOG_WIDTH, 1, c->text_format_secondary);
    pharaoh_text_input_field_initialize(c->text_input_state,
        PHARAOH_STARTUP_TEXT_FIELD_ENTRY_5,
        c->text_buffers->field_5_buffer,
        PHARAOH_STARTUP_FIELD_5_MAX_LENGTH,
        PHARAOH_STARTUP_FIELD_5_WIDTH, 0, c->text_format_primary);
    pharaoh_text_input_field_initialize(c->text_input_state,
        PHARAOH_STARTUP_TEXT_FIELD_ENTRY_6,
        c->text_buffers->field_6_buffer,
        PHARAOH_STARTUP_FIELD_6_MAX_LENGTH,
        PHARAOH_STARTUP_FIELD_6_WIDTH, 0, c->text_format_primary);
    pharaoh_text_input_field_initialize(c->text_input_state,
        PHARAOH_STARTUP_TEXT_FIELD_CHEAT,
        c->text_buffers->cheat_input_text,
        PHARAOH_STARTUP_CHEAT_MAX_LENGTH,
        PHARAOH_STARTUP_CHEAT_WIDTH, 0, c->text_format_primary);
    pharaoh_city_warning_clear_all(c->city_warning_clear);
}
