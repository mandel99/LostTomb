#include "pharaoh/ui/main_menu_open.h"

enum {
    MAIN_MENU_ACTION_CONTINUE = 0,
    MAIN_MENU_ACTION_CAMPAIGN = 1,
    MAIN_MENU_ACTION_LOAD_SAVE = 2,
    MAIN_MENU_ACTION_CUSTOM_MAPS = 3,
    MAIN_MENU_ACTION_CHANGE_PLAYER = 4,
    MAIN_MENU_WINDOW_CAMPAIGN = 0x17,
    MAIN_MENU_WINDOW_PLAYER_SELECTION = 0x2f,
    MAIN_MENU_WINDOW_MAIN = 0x30,
    MAIN_MENU_WINDOW_MAP_SELECTION = 0x3d,
    MAIN_MENU_WINDOW_GAMEPLAY = 1
};

void pharaoh_main_menu_dispatch_action(
    const PharaohMainMenuActionContext *context, int32_t action)
{
    PharaohMainMenuActionState *const state = context->state;
    const PharaohMainMenuActionOperations *const operations =
        context->operations;

    switch (action) {
    case MAIN_MENU_ACTION_CONTINUE:
        operations->stop_sound(operations->user_data);
        if (state->window_state->selected_save_available != 0) {
            *state->selection_flag_605a3c = 0;
            *state->selection_flag_6de515 = 0;
            operations->load_selected_save(operations->user_data,
                context->selected_save_path);
            *state->current_window_id = MAIN_MENU_WINDOW_GAMEPLAY;
        } else {
            *state->main_menu_field_7027ac = 0;
            *state->main_menu_field_7027b0 = 0;
            *state->campaign_mode_6fdc98 = 0;
            *state->current_window_id = MAIN_MENU_WINDOW_CAMPAIGN;
        }
        *state->redraw_requested = 1;
        break;

    case MAIN_MENU_ACTION_CAMPAIGN:
        if (state->window_state->menu_active == 0) {
            return;
        }
        if (operations->has_campaign_history(operations->user_data) == 0 &&
            operations->confirm_campaign_restart(operations->user_data) == 7) {
            return;
        }
        if (state->window_state->campaign_period_open != 0) {
            *state->current_window_id = MAIN_MENU_WINDOW_CAMPAIGN;
            *state->campaign_mode_6fdc98 = 1;
        } else {
            *state->selection_flag_605a3c = 0;
            *state->selection_flag_6de515 = 0;
            *state->map_mode_704dac = 2;
            *state->map_changed_704db0 = 1;
            *state->current_window_id = MAIN_MENU_WINDOW_MAP_SELECTION;
            operations->open_map_selection(operations->user_data);
        }
        *state->redraw_requested = 1;
        break;

    case MAIN_MENU_ACTION_LOAD_SAVE:
        if (state->window_state->save_directory_has_files != 0) {
            *state->selection_flag_605a3c = 0;
            *state->selection_flag_6de515 = 0;
            operations->open_saved_game_dialog(operations->user_data);
            *state->load_dialog_return_window = MAIN_MENU_WINDOW_MAIN;
        }
        break;

    case MAIN_MENU_ACTION_CUSTOM_MAPS:
        *state->campaign_mode_e38ebc = 2;
        *state->selection_flag_605a3c = 0;
        *state->selection_flag_6de515 = 0;
        *state->map_changed_704db0 = 0;
        *state->map_mode_704dac = 3;
        *state->current_window_id = MAIN_MENU_WINDOW_MAP_SELECTION;
        operations->open_map_selection(operations->user_data);
        *state->redraw_requested = 1;
        break;

    case MAIN_MENU_ACTION_CHANGE_PLAYER:
        operations->open_player_selection(operations->user_data);
        *state->current_window_id = MAIN_MENU_WINDOW_PLAYER_SELECTION;
        *state->redraw_requested = 1;
        break;

    default:
        break;
    }
}
