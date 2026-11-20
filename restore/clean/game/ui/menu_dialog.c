#include "pharaoh/ui/menu_dialog.h"

static void respond(uint8_t accepted, const PharaohMenuDialogState *state,
    const PharaohMenuDialogOperations *operations)
{
    *state->accepted = accepted;
    operations->respond(operations->user_data);
    *state->redraw_requested = 1;
    *state->current_window_id = *state->return_window_id;
}

void pharaoh_menu_dialog_accept(const PharaohMenuDialogState *state,
    const PharaohMenuDialogOperations *operations)
{
    respond(1, state, operations);
}

void pharaoh_menu_dialog_reject(const PharaohMenuDialogState *state,
    const PharaohMenuDialogOperations *operations)
{
    respond(0, state, operations);
}
