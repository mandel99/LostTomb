#ifndef PHARAOH_UI_LOAD_SAVE_ACTIONS_H
#define PHARAOH_UI_LOAD_SAVE_ACTIONS_H

#include <stdint.h>

typedef struct PharaohLoadSaveActionsState {
    uint8_t current_screen;
    uint8_t redraw_requested;
    uint8_t dialog_mode;
    uint8_t selection_active;
    uint32_t text_edit_state;
    uint32_t return_screen;
    int32_t selected_row;
    int32_t file_count;
    uint32_t selected_file;
    uint32_t first_visible_file;
    char load_name[64];
    char alternate_name[64];
    char default_name[64];
    const char *family_path;
    const char *save_suffix;
    void (*scan_save_files)(void *user_data, const char *pattern,
        uint8_t mode);
    void (*clear_text)(void *user_data, char *target, int32_t capacity,
        int32_t value);
    void (*prepare_text_editor)(void *user_data, int32_t mode);
    void (*prepare_text_cursor)(void *user_data);
    void (*prepare_text_selection)(void *user_data);
    const char *(*file_name)(void *user_data, int32_t index);
    void *user_data;
} PharaohLoadSaveActionsState;

void pharaoh_load_save_enter_load(PharaohLoadSaveActionsState *state);
void pharaoh_load_save_enter_alternate(PharaohLoadSaveActionsState *state);
void pharaoh_load_save_select_row(PharaohLoadSaveActionsState *state);

#endif
