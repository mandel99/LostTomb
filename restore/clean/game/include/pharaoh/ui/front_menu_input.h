#ifndef PHARAOH_UI_FRONT_MENU_INPUT_H
#define PHARAOH_UI_FRONT_MENU_INPUT_H

#include "pharaoh/ui/button_layout.h"
#include "pharaoh/ui/front_menu_action.h"
#include "pharaoh/ui/generic_button_focus.h"
#include "pharaoh/ui/generic_button_input.h"

typedef struct PharaohFrontMenuInputContext {
    PharaohGenericButton *buttons;
    PharaohGenericButtonFocusState *focus_state;
    PharaohButtonLayoutState *layout_state;
    const PharaohButtonLayoutOperations *layout_operations;
    PharaohGenericButtonInputState *input_state;
    PharaohFrontMenuActionContext *action_context;
    int32_t *focus_button_id;
} PharaohFrontMenuInputContext;

/* The window-zero input branch of native dispatcher 0x004D8310.
   Call only after the dispatcher has selected the title/front menu window. */
void pharaoh_front_menu_handle_input(
    PharaohFrontMenuInputContext *context);

#endif
