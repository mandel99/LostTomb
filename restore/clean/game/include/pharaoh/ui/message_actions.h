#ifndef PHARAOH_UI_MESSAGE_ACTIONS_H
#define PHARAOH_UI_MESSAGE_ACTIONS_H

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

void pharaoh_menu_message_ten(PharaohMenuMessageState *state);
void pharaoh_menu_message_go_to(PharaohMenuMessageState *state);
void pharaoh_menu_message_help(PharaohMenuMessageState *state);
void pharaoh_menu_message_scroll(PharaohMenuMessageState *state);
void pharaoh_menu_message_open(PharaohMenuMessageState *state);
void pharaoh_menu_message_delete(PharaohMenuMessageState *state);
void pharaoh_menu_message_delete_read(PharaohMenuMessageState *state);
void pharaoh_menu_overlay_select(PharaohMenuMessageState *state);

#endif
