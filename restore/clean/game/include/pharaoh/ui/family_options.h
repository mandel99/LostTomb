#ifndef PHARAOH_UI_FAMILY_OPTIONS_H
#define PHARAOH_UI_FAMILY_OPTIONS_H

#include <stdint.h>

typedef struct PharaohFamilyOptionsState {
    uint8_t saved_game_available;
    uint8_t campaign_available;
    uint8_t campaign_resume_mode;
    uint8_t editor_available;
    uint8_t current_screen;
    uint8_t redraw_requested;
    uint8_t game_loaded;
    uint32_t selected_family;
    uint32_t mission_selection_pending;
    uint32_t map_selection_mode;
    uint32_t map_selection_source;
    uint32_t editor_next_window;
    uint32_t editor_mode;
    uint32_t transient_a;
    uint32_t transient_b;
    void (*stop_menu_sound)(void *user_data);
    void (*load_saved_family)(void *user_data);
    uint8_t (*campaign_has_entries)(void *user_data);
    const char *(*select_text)(void *user_data, int32_t group,
        int32_t index);
    int32_t (*run_text_dialog)(void *user_data, const char *first,
        const char *second, int32_t mode);
    void (*show_map_selection)(void *user_data);
    void (*prepare_editor_map_name)(void *user_data);
    void *user_data;
} PharaohFamilyOptionsState;

void pharaoh_family_action_cb800(PharaohFamilyOptionsState *state);
void pharaoh_family_action_cb860(PharaohFamilyOptionsState *state);
void pharaoh_family_action_cb910(PharaohFamilyOptionsState *state);
void pharaoh_family_action_cb940(PharaohFamilyOptionsState *state);

#endif
