#ifndef PHARAOH_UI_MENU_BUTTON_LAYOUT_INIT_H
#define PHARAOH_UI_MENU_BUTTON_LAYOUT_INIT_H

#include "pharaoh/ui/generic_button_input.h"
#include "pharaoh/ui/window_layout_key.h"

typedef struct PharaohMenuLayoutNode {
    int32_t button_index;
    int32_t reserved;
    int32_t left;
    int32_t top;
    int32_t width;
    int32_t height;
    struct PharaohMenuLayoutNode *next;
} PharaohMenuLayoutNode;

/* Original entry 0x004D66F0. Node indices are one-based and must refer to an
   existing button, matching the native linked-list data contract. */
void pharaoh_menu_buttons_initialize(PharaohGenericButton *buttons,
    const PharaohMenuLayoutNode *layout_nodes);

/* Same 004D66F0 rules, borrowing the actual 0x1C-byte layout nodes.
   relative_x is the one-based button index; values_08 are left/top/width/
   height. Neither the list nor its bounds are copied to a normalized list. */
void pharaoh_menu_buttons_initialize_actual(PharaohGenericButton *buttons,
    const PharaohUiLayoutNode *layout_nodes);

#endif
