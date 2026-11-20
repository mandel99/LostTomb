#ifndef PHARAOH_UI_CUSTOM_MAP_SELECTION_H
#define PHARAOH_UI_CUSTOM_MAP_SELECTION_H

#include "pharaoh/runtime/file_list.h"
#include "pharaoh/scenario/campaign_file.h"
#include "pharaoh/scenario/scenario_preview.h"

enum { PHARAOH_CUSTOM_MAP_VISIBLE_ROWS = 15 };

typedef struct PharaohCustomMapSelectionState {
    PharaohFileList *files;
    int32_t selected_file;
    int32_t first_visible_file;
    int32_t visible_rows;
    int32_t campaign_mode;
    int32_t warning_text_id;
    char selected_name[64];
    int32_t mission_ids[PHARAOH_CAMPAIGN_INDEX_CAPACITY];
    PharaohScenarioPreview preview;
    int32_t preview_invasion_count;
    int32_t preview_current_event;
    uint8_t preview_invasion_search;
    uint8_t show_flag_a;
    uint8_t show_flag_b;
} PharaohCustomMapSelectionState;

typedef struct PharaohMapSelectionOperations {
    void *user_data;
    int32_t (*load_custom_map)(void *user_data, const char *path,
                              int32_t mode);
    int32_t (*load_campaign_mission)(void *user_data, int32_t package_index,
                                    int32_t skip_initialize);
    void (*play_restore_sound)(void *user_data);
    void (*prepare_start)(void *user_data, int32_t value);
    void (*load_selected_scenario)(void *user_data, const char *name,
                                  int32_t campaign_kind);
    void (*enter_campaign)(void *user_data);
    void (*refresh_difficulty_a)(void *user_data);
    void (*refresh_difficulty_b)(void *user_data);
    void (*refresh_difficulty_c)(void *user_data);
    void (*reload_map_assets)(void *user_data);
    int32_t (*adjust_value)(void *user_data, int32_t value,
                           int32_t difficulty_value);
} PharaohMapSelectionOperations;

typedef struct PharaohMapSelectionStartState {
    uint8_t *game_is_replay;
    uint8_t *current_window_id;
    uint8_t *transition_mode;
    uint8_t *redraw_requested;
} PharaohMapSelectionStartState;

typedef struct PharaohMapSelectionBackOperations {
    void *user_data;
    void (*show_main_menu)(void *user_data);
} PharaohMapSelectionBackOperations;

/* Native 0x0041F810 in custom-map mode: load maps\\*.map and sort names. */
int32_t pharaoh_custom_map_selection_load(
    PharaohCustomMapSelectionState *state);

/* Native 0x0041F810 / 0x0041DF70 path stores campaign names in source order;
   each row's selected index is its mission ID. */
int32_t pharaoh_campaign_mission_selection_load(
    PharaohCustomMapSelectionState *state, const PharaohCampaign *campaign);

/* Complete typed C translation of native 0x0041F870, including the retained
   selected index and initial 0x0041F670 activation. */
void pharaoh_map_selection_show(
    PharaohCustomMapSelectionState *state, const PharaohCampaign *campaign,
    const PharaohMapSelectionOperations *operations,
    uint8_t *current_window_id, uint8_t *redraw_requested);

/* Complete typed C translations of native 0x0041DFC0, 0x0041DFE0 and
   0x0041E010 for the clean list representation. */
int32_t pharaoh_map_selection_entry_mission(
    const PharaohCustomMapSelectionState *state, int32_t index);
int32_t pharaoh_map_selection_entry_kind(
    const PharaohCustomMapSelectionState *state, int32_t index);
const char *pharaoh_map_selection_entry_name(
    const PharaohCustomMapSelectionState *state, int32_t index);

/* Complete typed C translations of native 0x0041E140, 0x0041E170 and
   0x0041F210. */
int32_t pharaoh_map_selection_difficulty_buttons_visible(
    int32_t returns_to_front_menu, int32_t campaign_mode,
    int32_t map_mode);
int32_t pharaoh_map_selection_image_button_count(
    const PharaohCustomMapSelectionState *state);
int32_t pharaoh_map_selection_generic_button_count(int32_t campaign_mode);

/* Complete typed C translation of native 0x0041F670. */
int32_t pharaoh_map_selection_activate(
    PharaohCustomMapSelectionState *state, int32_t visible_row,
    const PharaohMapSelectionOperations *operations,
    uint8_t *redraw_requested);

/* Complete typed C translation of native 0x0041F5C0. */
int32_t pharaoh_map_selection_start(
    const PharaohCustomMapSelectionState *selection,
    const PharaohMapSelectionOperations *operations,
    const PharaohMapSelectionStartState *state);

/* Complete typed C translation of native 0x0041F650. */
void pharaoh_map_selection_back(
    int32_t returns_to_front_menu, uint8_t *current_window_id,
    uint8_t *redraw_requested,
    const PharaohMapSelectionBackOperations *operations);

/* Complete typed C translation of native 0x0041E0E0. */
void pharaoh_map_selection_toggle_mode(uint32_t *map_mode,
                                       uint8_t *redraw_requested);

/* Complete typed C translation of native 0x00402E50. */
void pharaoh_map_selection_adjust_difficulty(
    int32_t decrement, int32_t *difficulty, uint8_t current_window_id,
    int32_t scenario_value, const int32_t difficulty_values[5],
    int32_t *player_value, uint8_t *redraw_requested,
    const PharaohMapSelectionOperations *operations);

/* Selected-list-index calculation used by the 0x0041F670 row callback. */
int32_t pharaoh_custom_map_selection_select_visible(
    PharaohCustomMapSelectionState *state, int32_t visible_row);

/* Native arrow callback 0x0041F460; direction 0 is up, 1 is down. */
int32_t pharaoh_custom_map_selection_scroll(
    PharaohCustomMapSelectionState *state, int32_t direction);

/* Complete typed C translation of native 0x0041E1B0 position arithmetic.
   Returns zero when no thumb is drawn. */
int32_t pharaoh_map_selection_scroll_thumb_offset(
    const PharaohCustomMapSelectionState *state, int32_t *offset);

/* Native scrollbar callback 0x0041F4D0, for custom-map mode (0). */
int32_t pharaoh_custom_map_selection_drag_scrollbar(
    PharaohCustomMapSelectionState *state, int32_t mouse_x, int32_t mouse_y,
    int32_t origin_x, int32_t origin_y);

#endif
