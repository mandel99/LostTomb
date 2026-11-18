#ifndef PHARAOH_RUNTIME_INPUT_TRANSIENT_RESET_H
#define PHARAOH_RUNTIME_INPUT_TRANSIENT_RESET_H

#include <stdint.h>

typedef struct PharaohInputTransientResetState {
    /* Native DWORD store is observed only here; its meaning is unresolved. */
    uint32_t unclassified_dword;
    uint8_t mouse_left_clicked;
    uint8_t mouse_right_clicked;
} PharaohInputTransientResetState;

/* Original entry 0x004CF1F0. Stores are kept in native order. */
void pharaoh_input_transient_reset(
    PharaohInputTransientResetState *state);

#endif
