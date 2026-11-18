#ifndef PHARAOH_UI_MAIN_MENU_OPEN_H
#define PHARAOH_UI_MAIN_MENU_OPEN_H

#include "pharaoh/runtime/file_loading.h"
#include "pharaoh/scenario/campaign_start.h"

typedef struct PharaohMainMenuWindowState {
    uint8_t campaign_period_open;
    uint8_t menu_active;
    uint8_t save_directory_has_files;
    uint8_t selected_save_available;
    uint8_t active_window;
    uint8_t redraw_requested;
} PharaohMainMenuWindowState;

typedef struct PharaohMainMenuOpenOperations {
    void *user_data;
    PharaohReadOnlyFile *(*open_file)(void *user_data, const char *filename,
                                      const char *mode);
    int32_t (*close_file)(void *user_data, PharaohReadOnlyFile *file);
    int32_t (*count_matching_files)(void *user_data, const char *pattern,
                                    int32_t search_mode);
} PharaohMainMenuOpenOperations;

typedef struct PharaohMainMenuOpenContext {
    const PharaohCampaignIndex *campaign_index;
    PharaohMainMenuWindowState *state;
    const char *selected_save_filename;
    const char *save_directory;
    const PharaohMainMenuOpenOperations *operations;
} PharaohMainMenuOpenContext;

typedef struct PharaohMainMenuActionState {
    PharaohMainMenuWindowState *window_state;
    uint8_t *current_window_id;
    uint8_t *redraw_requested;
    uint32_t *selection_flag_605a3c;
    uint8_t *selection_flag_6de515;
    uint32_t *main_menu_field_7027ac;
    uint32_t *main_menu_field_7027b0;
    uint32_t *campaign_mode_6fdc98;
    uint32_t *campaign_mode_e38ebc;
    uint32_t *map_mode_704dac;
    uint32_t *map_changed_704db0;
    uint32_t *load_dialog_return_window;
} PharaohMainMenuActionState;

typedef struct PharaohMainMenuActionOperations {
    void *user_data;
    void (*stop_sound)(void *user_data);
    void (*load_selected_save)(void *user_data, const char *path);
    int32_t (*has_campaign_history)(void *user_data);
    int32_t (*confirm_campaign_restart)(void *user_data);
    void (*open_map_selection)(void *user_data);
    void (*open_saved_game_dialog)(void *user_data);
    void (*open_player_selection)(void *user_data);
} PharaohMainMenuActionOperations;

typedef struct PharaohMainMenuActionContext {
    PharaohMainMenuActionState *state;
    const PharaohMainMenuActionOperations *operations;
    const char *selected_save_path;
} PharaohMainMenuActionContext;

/* Native entry 0x004CB6A0. Updates menu availability and opens screen 0x30. */
void pharaoh_main_menu_window_show(const PharaohMainMenuOpenContext *context);

/* Native state-0x30 callbacks from table 0x005E0408, action IDs 0 through 4. */
void pharaoh_main_menu_dispatch_action(
    const PharaohMainMenuActionContext *context, int32_t action);

#endif
