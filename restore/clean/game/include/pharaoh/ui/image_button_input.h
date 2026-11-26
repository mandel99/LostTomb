#ifndef PHARAOH_UI_IMAGE_BUTTON_INPUT_H
#define PHARAOH_UI_IMAGE_BUTTON_INPUT_H

#include "pharaoh/ui/generic_button_focus.h"

typedef struct PharaohImageButtonInputState {
    int32_t cursor_x, cursor_y, hit_x, hit_y;
    uint8_t left_pressed, right_pressed, left_down;
    uint8_t redraw_requested;
    uint32_t primary_parameter, secondary_parameter;
    void *user_data;
    void (*dispatch)(void *user_data, uint32_t action);
} PharaohImageButtonInputState;

/* Native00425990 reads the trigger AFTER the counted records for its initial
 * gating. trailing_trigger must come from that actual adjacent native storage;
 * it is not the hovered button's trigger. Callback DWORDs are dispatch tokens,
 * never process addresses. Native0047D350's discarded zero result has no effect. */
int32_t pharaoh_image_buttons_handle_mouse(int32_t x, int32_t y,
    PharaohImageButtonRecord *buttons, int32_t count, int16_t trailing_trigger,
    PharaohImageButtonInputState *state);

#endif
