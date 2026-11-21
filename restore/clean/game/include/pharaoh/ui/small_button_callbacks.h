#ifndef PHARAOH_UI_SMALL_BUTTON_CALLBACKS_H
#define PHARAOH_UI_SMALL_BUTTON_CALLBACKS_H

#include <stdint.h>

typedef struct PharaohMapViewPoint {
    int16_t x;
    int16_t y;
    uint8_t reserved[28];
} PharaohMapViewPoint;

typedef struct PharaohSmallButtonState {
    uint32_t secondary_parameter;
    uint32_t timer_active;
    uint32_t timer_first;
    uint32_t timer_mode;
    uint32_t timer_value;
    uint32_t timer_last;
    int32_t message_id;
    uint32_t map_view_commit_enabled;
    uint32_t selected_map_point;
    int16_t hit_scroll_x;
    int16_t hit_scroll_y;
    int16_t committed_map_point;
    PharaohMapViewPoint *map_points;
    uint8_t current_window;
    uint8_t redraw_requested;
    void (*open_message)(void *user_data, int32_t message,
        int32_t background, int32_t templated);
    void (*start_timer)(void *user_data, int32_t first, int32_t mode,
        int32_t duration);
    void (*refresh_map_point)(void *user_data);
    void *user_data;
} PharaohSmallButtonState;

void pharaoh_small_button_show_message_29(PharaohSmallButtonState *state);
void pharaoh_small_button_select_secondary_window(
    PharaohSmallButtonState *state);
void pharaoh_small_button_show_message_32(PharaohSmallButtonState *state);
void pharaoh_small_button_start_timer(PharaohSmallButtonState *state);
void pharaoh_small_button_show_selected_message(
    PharaohSmallButtonState *state);
void pharaoh_small_button_commit_map_view(PharaohSmallButtonState *state);

#endif
