#ifndef PHARAOH_RUNTIME_PENDING_UI_STATE_H
#define PHARAOH_RUNTIME_PENDING_UI_STATE_H

#include <stdint.h>

typedef struct PharaohPendingUiState {
    int32_t *pending_selector;
    const int32_t *pending_value_a;
    const int32_t *pending_value_b;
    const int32_t *pending_value_c;
    int32_t *published_value_a;
    int32_t *published_value_b;
    int32_t *published_value_c;
    uint8_t *redraw_status;
} PharaohPendingUiState;

/* Typed translation of native pending-state commit 0x004CA930. */
int32_t pharaoh_pending_ui_state_commit(const PharaohPendingUiState *state);

#endif
