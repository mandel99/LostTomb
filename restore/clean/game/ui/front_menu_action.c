#include "pharaoh/ui/front_menu_action.h"

enum {
    FRONT_MENU_ACTION_PLAYER_SELECTION = 0,
    FRONT_MENU_ACTION_SUPPORT = 1,
    FRONT_MENU_ACTION_CUSTOM_MAPS = 2,
    FRONT_MENU_ACTION_HIGH_SCORES = 3,
    FRONT_MENU_ACTION_EXIT = 4,
    FRONT_MENU_HIGH_SCORES_WINDOW = 0x53
};

void pharaoh_front_menu_dispatch_action(void *user_data)
{
    PharaohFrontMenuActionContext *const context = user_data;
    PharaohFrontMenuActionState *const state = context->state;
    const PharaohFrontMenuActionOperations *const operations =
        context->operations;
    const int32_t action = state->input_state->selected_primary_parameter;

    *state->action_active = 0;
    *state->menu_action_reset = 1;

    switch (action) {
    case FRONT_MENU_ACTION_PLAYER_SELECTION:
        operations->open_player_selection(operations->user_data);
        *state->continue_campaign_pending = 0;
        break;

    case FRONT_MENU_ACTION_SUPPORT:
        operations->show_support_website(operations->user_data);
        break;

    case FRONT_MENU_ACTION_CUSTOM_MAPS:
        *state->campaign_flow_mode = 2;
        *state->scenario_menu_changed = 1;
        *state->save_selection_mode = 0;
        *state->continue_campaign_pending = 0;
        *state->new_campaign_field_a = 0;
        *state->new_campaign_field_b = 3;
        operations->open_custom_map_selection(operations->user_data);
        break;

    case FRONT_MENU_ACTION_HIGH_SCORES:
        operations->open_high_scores(operations->user_data);
        *state->current_window_id = FRONT_MENU_HIGH_SCORES_WINDOW;
        break;

    case FRONT_MENU_ACTION_EXIT:
        operations->request_window_exit(operations->user_data);
        break;

    default:
        break;
    }
}

void pharaoh_front_menu_ignore_secondary_action(void *user_data)
{
    (void)user_data;
}

void pharaoh_front_menu_confirm_exit(PharaohFrontMenuExitState *state,
    const PharaohFrontMenuExitOperations *operations)
{
    const char *body;
    const char *title;
    int32_t result;

    operations->stop_video(operations->user_data);
    body = operations->select_body_text(operations->user_data, 5, 0);
    title = operations->select_title_text(operations->user_data, 5, 1);
    result = operations->show_message_box(operations->user_data,
        title, body, 4);
    *state->mouse_button_bits = 0;
    if (result == 6) {
        *state->post_close_requested = 1;
        *state->redraw_request = 1;
    }
}
