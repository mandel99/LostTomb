#ifndef PHARAOH_RUNTIME_CRT_NEW_HANDLER_DISPATCH_H
#define PHARAOH_RUNTIME_CRT_NEW_HANDLER_DISPATCH_H

#include <stdint.h>

typedef int32_t (*PharaohCrtNewHandler)(uint32_t requested_bytes);

typedef struct PharaohCrtNewHandlerState {
    PharaohCrtNewHandler handler;
} PharaohCrtNewHandlerState;

/* Complete typed leaf translation of native 0x005653D0. */
uint32_t pharaoh_crt_invoke_new_handler(
    const PharaohCrtNewHandlerState *state, uint32_t requested_bytes);

#endif
