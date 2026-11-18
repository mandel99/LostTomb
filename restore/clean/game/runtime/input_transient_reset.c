#include "pharaoh/runtime/input_transient_reset.h"

void pharaoh_input_transient_reset(
    PharaohInputTransientResetState *state)
{
    state->unclassified_dword = UINT32_C(0);
    state->mouse_left_clicked = UINT8_C(0);
    state->mouse_right_clicked = UINT8_C(0);
}
