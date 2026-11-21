#include "pharaoh/ui/campaign_menu.h"

enum { CAMPAIGN_MENU_WINDOW = 0x17 };

int32_t pharaoh_campaign_menu_period_valid(int32_t period)
{
    return period >= 0 && period < 9;
}

uint32_t pharaoh_campaign_menu_period_lock_mask(uint32_t period)
{
    return period & UINT32_C(0xffffff00);
}

int32_t pharaoh_campaign_menu_image_button_count(
    uint32_t campaign_mode, uint32_t selected_period,
    uint32_t latest_period)
{
    if (campaign_mode == 0) return selected_period == latest_period;
    return ((int8_t)pharaoh_campaign_menu_period_lock_mask(selected_period)
        != 0) ? 0 : 2;
}

void pharaoh_campaign_menu_select_period(
    PharaohCampaignMenuState *state, uint32_t period)
{
    *state->redraw_requested = 1;
    *state->selected_period = period;
}

int32_t pharaoh_campaign_menu_open_individual_missions(
    PharaohCampaignMenuState *state,
    const PharaohCampaignMenuOperations *operations)
{
    if (*state->current_window_id != CAMPAIGN_MENU_WINDOW ||
        *state->campaign_mode != 1) return 0;
    *state->map_changed = 1;
    *state->returns_to_front_menu = 0;
    *state->selection_flag = 0;
    *state->map_mode = 2;
    operations->show_individual_missions(operations->user_data);
    *state->redraw_requested = 1;
    return 1;
}

int32_t pharaoh_campaign_menu_open_campaigns(
    PharaohCampaignMenuState *state)
{
    if (*state->current_window_id == CAMPAIGN_MENU_WINDOW) return 0;
    *state->current_window_id = CAMPAIGN_MENU_WINDOW;
    *state->campaign_mode = 1;
    *state->redraw_requested = 1;
    return 1;
}

int32_t pharaoh_campaign_menu_start_selected(
    PharaohCampaignMenuState *state,
    const PharaohCampaignMenuOperations *operations)
{
    const uint32_t period = *state->selected_period;
    const uint32_t mode = *state->campaign_mode;

    if (mode == 0 && period != *state->latest_period) return 0;

    *state->mission_id = 0;
    *state->returns_to_front_menu = 0;
    *state->selection_flag = 0;
    *state->campaign_started = 1;
    *state->redraw_requested = 1;
    *state->selection_kind = (mode != 0) + 1;

    if (period < 5) {
        operations->play_period_intro(operations->user_data, period);
    }
    operations->start_campaign(operations->user_data, (int8_t)period);
    if (*state->mission_id == UINT8_C(0x7f)) {
        *state->current_window_id = UINT8_C(0x34);
    } else {
        operations->begin_briefing(operations->user_data);
    }
    return 1;
}
