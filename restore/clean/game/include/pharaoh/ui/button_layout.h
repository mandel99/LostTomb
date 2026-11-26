#ifndef PHARAOH_UI_BUTTON_LAYOUT_H
#define PHARAOH_UI_BUTTON_LAYOUT_H

#include <stdint.h>

#include "pharaoh/ui/generic_button_input.h"
#include "pharaoh/ui/menu_button_layout_init.h"

typedef struct PharaohMenuLayoutSlot {
    PharaohMenuLayoutNode *node;
    int32_t initialized;
} PharaohMenuLayoutSlot;

typedef struct PharaohLayoutHighlightState {
    int32_t edit_mode;
    int32_t mouse_x, mouse_y;
    int32_t marked_x, marked_y;
    int32_t marked_key_x, marked_key_y;
    int32_t marked_width, marked_height;
    void *user_data;
    void (*clear_highlight)(void *user_data);
    void (*draw_border)(void *user_data, int32_t style, int32_t x,
        int32_t y, int32_t width, int32_t height, uint32_t color);
} PharaohLayoutHighlightState;

void pharaoh_ui_layout_highlight(PharaohLayoutHighlightState *state,
    int32_t kind, int32_t x, int32_t y, int32_t width, int32_t height,
    int32_t base_x, int32_t base_y, int32_t offset_x, int32_t offset_y);

typedef struct PharaohButtonLayoutState {
    int32_t window_key;
    int32_t window_seven_layout_base;
    int32_t highlight_mode;
    PharaohMenuLayoutSlot *slots_by_key;

    const PharaohGenericButton *selected_button;
    int32_t selected_button_index;
    int32_t selected_left;
    int32_t selected_top;
    int32_t selected_width;
    int32_t selected_height;
    int32_t highlighted_button_index;
    int32_t highlight_auxiliary_state;
} PharaohButtonLayoutState;

typedef struct PharaohButtonLayoutOperations {
    void *context;
    void (*set_clip_rectangle)(void *context,
        int32_t left, int32_t top, int32_t width, int32_t height);
    void (*highlight_actor_panel)(void *context, int32_t x);
    void (*draw_highlight_rectangle)(void *context, int32_t style,
        int32_t x, int32_t y, int32_t width, int32_t height,
        uint32_t color);
} PharaohButtonLayoutOperations;

/* Original entry 0x004D6740. button_count is intentionally unused by the
   native function; button_index is a one-based hit result from the caller. */
int32_t pharaoh_ui_layout_buttons(PharaohButtonLayoutState *state,
    const PharaohButtonLayoutOperations *operations,
    int32_t origin_x, int32_t origin_y,
    PharaohGenericButton *buttons, int32_t button_count,
    int32_t button_index);

#endif
