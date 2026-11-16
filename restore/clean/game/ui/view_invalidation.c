#include "pharaoh/ui/view_invalidation.h"

void pharaoh_view_invalidate_if_editing(PharaohViewState *state)
{
    if (state->layout_edit_active) {
        state->redraw_request = PHARAOH_REDRAW_ALL;
    }
}
