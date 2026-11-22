#ifndef PHARAOH_RUNTIME_MOUSE_UPDATE_H
#define PHARAOH_RUNTIME_MOUSE_UPDATE_H

#include <stdint.h>

#include "pharaoh/runtime/sound_effect_request.h"

typedef struct PharaohMousePoint {
    int32_t x;
    int32_t y;
} PharaohMousePoint;

typedef struct PharaohMouseUpdateState {
    uint32_t cursor_sample_enabled;
    int32_t cursor_screen_x;
    int32_t cursor_screen_y;
    int16_t cursor_sample_x;
    int16_t cursor_sample_y;

    int32_t drag_state;
    int32_t drag_anchor_x;
    int32_t drag_anchor_y;
    int32_t logical_x;
    int32_t logical_y;
    int32_t previous_logical_x;
    int32_t previous_logical_y;

    uint8_t raw_button_bits;
    uint8_t left_button_down;
    uint8_t right_button_down;
    uint8_t previous_left_button_down;
    uint8_t previous_right_button_down;
    uint8_t left_button_pressed;
    uint8_t left_button_released;
    uint8_t right_button_pressed;
    uint8_t right_button_released;
    uint8_t right_button_auxiliary;

    uint32_t pointer_changed;
    uint32_t button_state_changed;
    int32_t cursor_scale_x;
    int32_t cursor_scale_y;
    int32_t viewport_left;
    int32_t viewport_top;
    int32_t viewport_right;
    int32_t viewport_bottom;
    uint32_t current_time;
    uint32_t last_pointer_change_time;
    uint32_t time_since_pointer_change;
} PharaohMouseUpdateState;

typedef struct PharaohMouseUpdateOperations {
    void *context;
    /* The native code ignores GetCursorPos's return value. This operation
       supplies the resulting point; callers must provide a valid sample. */
    void (*query_cursor_position)(void *context, PharaohMousePoint *point);
} PharaohMouseUpdateOperations;

typedef enum PharaohMouseUpdateStatus {
    PHARAOH_MOUSE_UPDATE_COMPLETE,
    PHARAOH_MOUSE_UPDATE_SIGNED_DIVIDE_OVERFLOW
} PharaohMouseUpdateStatus;

/* Original entry 0x004CF210. State widths follow the native loads/stores;
   the sound request is issued on the left-button down transition. */
PharaohMouseUpdateStatus pharaoh_mouse_update(
    PharaohMouseUpdateState *state,
    const PharaohMouseUpdateOperations *operations,
    PharaohSoundEffectRequestState *sound_state,
    const PharaohSoundEffectRequestOperations *sound_operations);

int32_t pharaoh_mouse_inside_rect(int32_t mouse_x, int32_t mouse_y,
    int32_t x, int32_t y, int32_t width, int32_t height);

#endif
