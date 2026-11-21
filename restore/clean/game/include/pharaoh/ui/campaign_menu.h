#ifndef PHARAOH_UI_CAMPAIGN_MENU_H
#define PHARAOH_UI_CAMPAIGN_MENU_H

#include <stdint.h>

typedef struct PharaohCampaignMenuState {
    uint8_t *current_window_id;
    uint8_t *redraw_requested;
    uint32_t *selected_period;
    uint32_t *latest_period;
    uint32_t *campaign_mode;
    uint32_t *map_changed;
    uint32_t *returns_to_front_menu;
    uint8_t *selection_flag;
    uint32_t *map_mode;
    uint32_t *campaign_started;
    uint8_t *mission_id;
    uint32_t *selection_kind;
} PharaohCampaignMenuState;

typedef struct PharaohCampaignMenuOperations {
    void *user_data;
    void (*show_individual_missions)(void *user_data);
    void (*play_period_intro)(void *user_data, uint32_t period);
    void (*start_campaign)(void *user_data, int8_t period);
    void (*begin_briefing)(void *user_data);
} PharaohCampaignMenuOperations;

/* Complete typed C translations of native 0x0041BA00, 0x0041BAD0,
   0x0041BB30 and 0x0041BB10. */
int32_t pharaoh_campaign_menu_period_valid(int32_t period);
uint32_t pharaoh_campaign_menu_period_lock_mask(uint32_t period);
int32_t pharaoh_campaign_menu_image_button_count(
    uint32_t campaign_mode, uint32_t selected_period,
    uint32_t latest_period);
void pharaoh_campaign_menu_select_period(
    PharaohCampaignMenuState *state, uint32_t period);

/* Complete typed C translations of the tab callbacks 0x0041C310 and
   0x0041C350. Return nonzero only when the native callback changes state. */
int32_t pharaoh_campaign_menu_open_individual_missions(
    PharaohCampaignMenuState *state,
    const PharaohCampaignMenuOperations *operations);
int32_t pharaoh_campaign_menu_open_campaigns(
    PharaohCampaignMenuState *state);

/* Complete typed C translation of native 0x0041BB70.  The three deep
   endpoints retain their native call order through explicit operations. */
int32_t pharaoh_campaign_menu_start_selected(
    PharaohCampaignMenuState *state,
    const PharaohCampaignMenuOperations *operations);

#endif
