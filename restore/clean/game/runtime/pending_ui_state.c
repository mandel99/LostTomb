#include "pharaoh/runtime/pending_ui_state.h"

int32_t pharaoh_pending_ui_state_commit(const PharaohPendingUiState *state)
{
    int32_t selector;
    int32_t value_a;
    int32_t value_b;
    int32_t value_c;
    uint8_t redraw_status;

    selector = *state->pending_selector;
    if (selector < 1) {
        return 0;
    }

    value_a = *state->pending_value_a;
    value_b = *state->pending_value_b;
    value_c = *state->pending_value_c;
    redraw_status = *state->redraw_status;

    *state->published_value_a = value_a;
    *state->pending_selector = 0;
    *state->published_value_b = value_b;
    *state->published_value_c = value_c;
    if (redraw_status == 0) {
        *state->redraw_status = 2;
    }
    return 1;
}
