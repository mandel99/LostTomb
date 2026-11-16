#ifndef PHARAOH_UI_GENERIC_BUTTON_FOCUS_H
#define PHARAOH_UI_GENERIC_BUTTON_FOCUS_H

#include <stdint.h>

#include "pharaoh/ui/main_menu.h"

typedef struct PharaohGenericButtonFocusState {
    int32_t hit_mouse_y;
    int32_t hit_mouse_x;
} PharaohGenericButtonFocusState;

/* Original entry 0x00426450. Returns the first hit button as a one-based
   index and updates hit_mouse_y/x only when a rectangle contains the cursor. */
int32_t pharaoh_generic_buttons_get_focus(
    int32_t origin_x, int32_t origin_y,
    const PharaohMenuRect *rectangles, int32_t rectangle_count,
    int32_t cursor_x, int32_t cursor_y,
    PharaohGenericButtonFocusState *focus_state);

#endif
