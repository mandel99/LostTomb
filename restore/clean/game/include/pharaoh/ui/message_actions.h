#ifndef PHARAOH_UI_MESSAGE_ACTIONS_H
#define PHARAOH_UI_MESSAGE_ACTIONS_H

#include "pharaoh/ui/rich_text.h"
#include <stdint.h>

typedef struct PharaohMenuMessageRecord {
    uint32_t first;
    uint32_t second;
    int16_t kind;
    int16_t message;
    uint8_t reserved_0c[2];
    uint8_t read;
    uint8_t source;
    int16_t template_id;
    uint8_t reserved_12[30];
} PharaohMenuMessageRecord;

typedef struct PharaohMenuMessageState {
    uint32_t button_parameter;
    int32_t scroll_index;
    int32_t scroll_reset;
    int32_t visible_count;
    int32_t message_count;
    int32_t selected_message;
    int32_t selected_detail;
    int32_t current_message;
    int32_t active_message_id;
    uint32_t message_kind;
    uint32_t message_source;
    uint32_t message_first;
    uint32_t message_second;
    uint32_t overlay_group;
    uint32_t overlay_secondary;
    uint32_t current_overlay;
    const uint32_t *overlay_values;
    uint8_t current_screen;
    uint8_t redraw_requested;
    PharaohMenuMessageRecord *messages;
    uint16_t (*message_category)(void *user_data, int32_t message);
    void (*close_message_panel)(void *user_data);
    void (*show_message)(void *user_data, int32_t message, int32_t value,
        uint8_t templated);
    uint32_t (*group_tile)(void *user_data, int32_t group);
    void (*go_to_tile)(void *user_data, uint32_t tile);
    void (*prepare_message)(void *user_data, int32_t message);
    void (*sort_messages)(void *user_data);
    void (*refresh_overlay)(void *user_data);
    void *user_data;
} PharaohMenuMessageState;

typedef struct PharaohMessageDialogCloseState {
    int32_t show_video;
    uint8_t previous_window;
    uint8_t current_window;
    int32_t message_category;
    uint8_t redraw_requested;
    void (*stop_video)(void *user_data);
    void *user_data;
} PharaohMessageDialogCloseState;

typedef struct PharaohMessageDialogOpenState {
    uint8_t *has_phrases;
    uint16_t *phrase_count;
    uint32_t *dialog_words_a; /* 200 dwords */
    uint32_t *dialog_words_b; /* 200 dwords */
    int32_t *message_id;
    uint32_t *message_parameter;
    uint32_t *dialog_state_aae0;
    uint32_t *dialog_state_b130;
    uint32_t *dialog_state_b138;
    uint32_t *dialog_state_b12c;
    uint32_t *dialog_visible;
    uint32_t *previous_window_id;
    const uint32_t *message_record_value;
    const uint32_t *event54_popup;
    const uint16_t *current_message_phrase_count;
    uint8_t *redraw_requested;
    uint8_t *current_window_id;
    PharaohRichTextState *rich_text;
} PharaohMessageDialogOpenState;

typedef struct PharaohMessageInputRow {
    int32_t top;
    int32_t bottom;
    int32_t left;
    int32_t right;
    int32_t value;
} PharaohMessageInputRow;

void pharaoh_menu_message_ten(PharaohMenuMessageState *state);
void pharaoh_menu_message_go_to(PharaohMenuMessageState *state);
void pharaoh_menu_message_help(PharaohMenuMessageState *state);
void pharaoh_menu_message_scroll(PharaohMenuMessageState *state);
void pharaoh_menu_message_open(PharaohMenuMessageState *state);
void pharaoh_menu_message_delete(PharaohMenuMessageState *state);
void pharaoh_menu_message_delete_read(PharaohMenuMessageState *state);
void pharaoh_menu_overlay_select(PharaohMenuMessageState *state);
void pharaoh_message_dialog_close(PharaohMessageDialogCloseState *state);
/* Native entry 0x004C5BF0. */
void pharaoh_message_dialog_show(PharaohMessageDialogOpenState *state,
    int32_t message_id, uint32_t parameter, uint8_t has_phrases);
int32_t pharaoh_message_count_empty_slots(const int16_t slots[1000]);
int32_t pharaoh_message_input_hit_test(uint8_t active,
    const PharaohMessageInputRow *rows, int32_t count,
    int32_t mouse_x, int32_t mouse_y);

#endif
