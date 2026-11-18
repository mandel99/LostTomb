#include "pharaoh/runtime/cursor_selection.h"

static const PharaohApplicationCursor native_cursor_by_selector[
    PHARAOH_CURSOR_SELECTOR_COUNT] = {
    PHARAOH_APPLICATION_CURSOR_DEFAULT,
    PHARAOH_APPLICATION_CURSOR_PERSON_DIRECTION,
    PHARAOH_APPLICATION_CURSOR_CLEAR_LAND,
    PHARAOH_APPLICATION_CURSOR_BOAT,
    PHARAOH_APPLICATION_CURSOR_BOAT_FIGURE_A,
    PHARAOH_APPLICATION_CURSOR_BOAT_FIGURE_B,
    PHARAOH_APPLICATION_CURSOR_BOAT_DOWN_A,
    PHARAOH_APPLICATION_CURSOR_BOAT_DOWN_B,
    PHARAOH_APPLICATION_CURSOR_SWORD,
    PHARAOH_APPLICATION_CURSOR_BOW,
    PHARAOH_APPLICATION_CURSOR_CART,
    PHARAOH_APPLICATION_CURSOR_PARTICLES_A,
    PHARAOH_APPLICATION_CURSOR_PARTICLES_B,
    PHARAOH_APPLICATION_CURSOR_PARTICLES_C,
    PHARAOH_APPLICATION_CURSOR_PARTICLES_D,
    PHARAOH_APPLICATION_CURSOR_COMBAT,
    PHARAOH_APPLICATION_CURSOR_BOAT_HORIZONTAL,
    PHARAOH_APPLICATION_CURSOR_BOAT_RIGHT,
    PHARAOH_APPLICATION_CURSOR_BOAT_BRUSH
};

void pharaoh_cursor_select(
    PharaohApplicationWindowState *state,
    const PharaohCursorSelectionOperations *operations,
    uint32_t selector)
{
    const uint32_t selection = selector < PHARAOH_CURSOR_SELECTOR_COUNT ?
        selector : PHARAOH_CURSOR_SELECTOR_SLOT_00;
    const PharaohApplicationCursor cursor_kind =
        native_cursor_by_selector[selection];

    state->current_cursor = state->cursors[cursor_kind];
    (void)operations->set_cursor(operations->context,
        state->current_cursor);
}
