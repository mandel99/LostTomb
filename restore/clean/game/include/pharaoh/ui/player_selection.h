#ifndef PHARAOH_UI_PLAYER_SELECTION_H
#define PHARAOH_UI_PLAYER_SELECTION_H

#include "pharaoh/runtime/file_list.h"
#include "pharaoh/ui/panel_frame.h"

#include <stddef.h>

enum {
    PHARAOH_PLAYER_SELECTION_VISIBLE_ROWS = 12,
    PHARAOH_PLAYER_NAME_CAPACITY = 32
};

typedef struct PharaohPlayerSelectionState {
    PharaohFileList *profiles;
    char player_name[PHARAOH_PLAYER_NAME_CAPACITY];
    int32_t selected_profile;
    int32_t first_visible_profile;
    int32_t selected_name_choice;
} PharaohPlayerSelectionState;

typedef struct PharaohPlayerSelectionDrawOperations {
    void *user_data;
    void (*draw_language)(void *user_data, int32_t group, int32_t entry,
        int32_t x, int32_t y, int32_t width, int32_t font, int32_t color);
    void (*draw_text)(void *user_data, const char *text, int32_t x,
        int32_t y, int32_t font, int32_t color);
    void (*draw_caret)(void *user_data, const char *text, int32_t x,
        int32_t y, int32_t font, int32_t color);
    void (*draw_sprite)(void *user_data, int32_t image, int32_t x, int32_t y);
    int32_t panel_image_base;
    int32_t inner_panel_image_base;
    int32_t button_image_base;
    int32_t arrow_image_base;
    int32_t scrollbar_thumb_image;
    int32_t accept_image;
} PharaohPlayerSelectionDrawOperations;

/* Native 0x00516B50/0x00516BB0: load Save\\*.dat, remove extensions,
   sort names case-insensitively and restore the previous profile index. */
int32_t pharaoh_player_selection_load_profiles(
    PharaohPlayerSelectionState *state, int32_t previous_profile);

/* Stateful selection helpers; native 0x00516890 scrolls the profile list. */
int32_t pharaoh_player_selection_select_visible(
    PharaohPlayerSelectionState *state, int32_t visible_row);
int32_t pharaoh_player_selection_scroll(
    PharaohPlayerSelectionState *state, int32_t direction);
int32_t pharaoh_player_selection_drag_scrollbar(
    PharaohPlayerSelectionState *state, int32_t mouse_y, int32_t origin_y);

/* Native 0x0041ACD0 and the path helpers 0x005169C0/0x00516A20. */
void pharaoh_player_name_trim(char name[PHARAOH_PLAYER_NAME_CAPACITY]);
int32_t pharaoh_player_selection_find_profile(
    const PharaohPlayerSelectionState *state, const char *name);
int32_t pharaoh_player_profile_paths(const char *name,
    char *directory, size_t directory_capacity,
    char *profile, size_t profile_capacity);

/* Complete typed translations of native 0x0040C890 and 0x0040C810. */
void pharaoh_player_name_choices_open(uint8_t *current_window_id,
    uint8_t *redraw_requested);
void pharaoh_player_name_choices_close(uint32_t *menu_action_reset,
    uint8_t *current_window_id, uint8_t *redraw_requested);

/* Native 0x00516630/0x00516720, with host supplied bitmap-asset drawing. */
void pharaoh_player_selection_draw(
    const PharaohPlayerSelectionState *state,
    const PharaohPlayerSelectionDrawOperations *operations,
    int32_t origin_x, int32_t origin_y, int32_t focused_button);

/* Native 0x00520B90. The background is the state-zero menu image drawn by
   0x00520B60; this function draws the centered name-entry panel above it. */
void pharaoh_player_name_input_draw(
    const PharaohPlayerSelectionState *state,
    const PharaohPlayerSelectionDrawOperations *operations,
    int32_t origin_x, int32_t origin_y, int32_t focused_button);

/* Native 0x00520D00: 42 catalog entries and the close button. */
void pharaoh_player_name_choices_draw(
    const PharaohPlayerSelectionState *state,
    const PharaohPlayerSelectionDrawOperations *operations,
    int32_t origin_x, int32_t origin_y, int32_t focused_button);

char *pharaoh_player_record_path(char *destination, const char *name);

#endif
