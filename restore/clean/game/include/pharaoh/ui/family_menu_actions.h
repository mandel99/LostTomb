#ifndef PHARAOH_UI_FAMILY_MENU_ACTIONS_H
#define PHARAOH_UI_FAMILY_MENU_ACTIONS_H

#include <stdint.h>

enum PharaohFamilyDialogAction {
    PHARAOH_FAMILY_DIALOG_DELETE_DYNASTY
};

typedef struct PharaohFamilyMenuState {
    uint8_t current_screen;
    uint8_t redraw_requested;
    uint32_t text_edit_state;
    uint32_t selected_row;
    uint32_t highlighted_row;
    uint32_t input_window;
    char family_name[32];
    const char *(*select_text)(void *user_data, int32_t group,
        int32_t index);
    void (*prepare_text_editor)(void *user_data, int32_t mode);
    void (*prepare_text_cursor)(void *user_data);
    void (*prepare_text_selection)(void *user_data);
    void (*show_dialog)(void *user_data, int32_t message,
        enum PharaohFamilyDialogAction action, int32_t style);
    void *user_data;
} PharaohFamilyMenuState;

void pharaoh_family_open_name_choices(PharaohFamilyMenuState *state);
void pharaoh_family_close_name_choices(PharaohFamilyMenuState *state);
void pharaoh_family_choose_name(PharaohFamilyMenuState *state);
void pharaoh_family_begin_new(PharaohFamilyMenuState *state);
void pharaoh_family_request_delete(PharaohFamilyMenuState *state);

#endif
