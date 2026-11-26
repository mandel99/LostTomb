#ifndef PHARAOH_UI_MENU_DROPDOWN_INPUT_H
#define PHARAOH_UI_MENU_DROPDOWN_INPUT_H

#include "pharaoh/ui/window_state_actions.h"
#include "pharaoh/ui/generic_button_focus.h"

typedef struct PharaohMenuDropdownAction {
    void (*invoke)(void *user_data);
    void *user_data;
} PharaohMenuDropdownAction;

typedef struct PharaohMenuDropdownHeaderData {
    /* Borrow the actual relocated header fields at native +8/+12. */
    const PharaohMenuDropdownRow *const *items;
    const int16_t *count;
    /* One real C action per item, replacing the native code pointer at +4. */
    const PharaohMenuDropdownAction *actions;
} PharaohMenuDropdownHeaderData;

typedef struct PharaohMenuDropdownInputContext {
    const PharaohMenuBarState *menu;
    const PharaohMenuDropdownFocusContext *focus;
    /* Parallel backing arrays, including the opaque prefix before headers.
       Every active_header must refer into header_storage. */
    const PharaohMenuBarFocusRecord *header_storage;
    const PharaohMenuDropdownHeaderData *header_data;
    const PharaohMenuDropdownRow **current_item; /* 705720 */
    uint32_t *button_parameter; /* D3B198 */
    uint32_t *keep_open; /* 70571C, live across the action call */
} PharaohMenuDropdownInputContext;

/* Complete00425F20. Requires the real relocated catalogue and complete C
   action bindings; raw native callback tokens are never executed. */
int32_t pharaoh_menu_dropdown_handle_input(
    const PharaohMenuDropdownInputContext *,
    const PharaohMenuBarFocusRecord *headers, int32_t count);

#endif
