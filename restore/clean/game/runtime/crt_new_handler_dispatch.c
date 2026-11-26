#include "pharaoh/runtime/crt_new_handler_dispatch.h"

uint32_t pharaoh_crt_invoke_new_handler(
    const PharaohCrtNewHandlerState *state, uint32_t requested_bytes)
{
    if (state->handler != 0 && state->handler(requested_bytes) != 0) {
        return 1;
    }
    return 0;
}
