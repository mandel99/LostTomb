#ifndef PHARAOH_UI_GENERIC_BUTTON_INPUT_H
#define PHARAOH_UI_GENERIC_BUTTON_INPUT_H

#include <stdint.h>

#include "pharaoh/ui/main_menu.h"

typedef enum PharaohGenericButtonTrigger {
    PHARAOH_GENERIC_BUTTON_TRIGGER_PRESS = 1,
    PHARAOH_GENERIC_BUTTON_TRIGGER_HOLD = 2,
    PHARAOH_GENERIC_BUTTON_TRIGGER_RELEASE = 3,
    PHARAOH_GENERIC_BUTTON_TRIGGER_LEFT_PRESS = 4
} PharaohGenericButtonTrigger;

typedef void (*PharaohGenericButtonAction)(void *context);

typedef struct PharaohGenericButton {
    PharaohMenuRect bounds;
    PharaohGenericButtonAction left_action;
    PharaohGenericButtonAction right_action;
    uint8_t activated;
    PharaohGenericButtonTrigger trigger;
    int32_t primary_parameter;
    int32_t secondary_parameter;
} PharaohGenericButton;

typedef struct PharaohGenericButtonInputState {
    int32_t origin_x;
    int32_t origin_y;
    int32_t cursor_x;
    int32_t cursor_y;
    uint8_t left_down;
    uint8_t left_pressed;
    uint8_t left_double_click;
    uint8_t left_released;
    uint8_t right_down;
    uint8_t right_pressed;
    uint8_t right_double_click;
    uint8_t right_released;
    int32_t hold_action_enabled;
    int32_t clear_drag_action;
    int32_t selected_primary_parameter;
    int32_t selected_secondary_parameter;
    void *action_context;
} PharaohGenericButtonInputState;

/* Original entry 0x004261D0. Returns the first activated button as a
   one-based index; callbacks are required when their trigger can select them. */
int32_t pharaoh_generic_buttons_handle_mouse(
    PharaohGenericButtonInputState *state,
    PharaohGenericButton *buttons,
    int32_t button_count);

#endif
