#ifndef PHARAOH_UI_FRONT_MENU_ACTION_H
#define PHARAOH_UI_FRONT_MENU_ACTION_H

#include "pharaoh/ui/generic_button_input.h"

#include <stdint.h>

typedef struct PharaohFrontMenuActionState {
    const PharaohGenericButtonInputState *input_state;
    uint8_t *action_active;
    uint32_t *menu_action_reset;
    uint32_t *continue_campaign_pending;
    uint32_t *campaign_flow_mode;
    uint32_t *map_selection_returns_to_front_menu;
    uint8_t *save_selection_mode;
    uint32_t *map_selection_campaign_mode;
    uint32_t *map_selection_mode;
    uint8_t *current_window_id;
} PharaohFrontMenuActionState;

typedef struct PharaohFrontMenuActionOperations {
    void *user_data;
    void (*open_player_selection)(void *user_data);
    void (*show_support_website)(void *user_data);
    void (*open_custom_map_selection)(void *user_data);
    void (*open_high_scores)(void *user_data);
    void (*request_window_exit)(void *user_data);
} PharaohFrontMenuActionOperations;

typedef struct PharaohFrontMenuActionContext {
    PharaohFrontMenuActionState *state;
    const PharaohFrontMenuActionOperations *operations;
} PharaohFrontMenuActionContext;

typedef struct PharaohFrontMenuExitState {
    uint8_t *mouse_button_bits;
    uint8_t *post_close_requested;
    uint8_t *redraw_request;
} PharaohFrontMenuExitState;

typedef struct PharaohFrontMenuExitOperations {
    void *user_data;
    void (*stop_video)(void *user_data);
    const char *(*select_body_text)(void *user_data, int32_t group,
        int32_t entry);
    const char *(*select_title_text)(void *user_data, int32_t group,
        int32_t entry);
    int32_t (*show_message_box)(void *user_data, const char *title,
        const char *body, uint32_t type);
} PharaohFrontMenuExitOperations;

/* Dispatches the five native front-menu action IDs using typed game/platform
   operations supplied by the owning runtime. */
void pharaoh_front_menu_dispatch_action(void *user_data);

/* The native secondary callback performs no work. */
void pharaoh_front_menu_ignore_secondary_action(void *user_data);

/* Native VK_ESCAPE branch used by front-menu action 4. */
void pharaoh_front_menu_confirm_exit(PharaohFrontMenuExitState *state,
    const PharaohFrontMenuExitOperations *operations);

#endif
