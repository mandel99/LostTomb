#ifndef PHARAOH_UI_VIEW_INVALIDATION_H
#define PHARAOH_UI_VIEW_INVALIDATION_H

#include <stdbool.h>

typedef enum PharaohRedrawRequest {
    PHARAOH_REDRAW_NONE = 0,
    PHARAOH_REDRAW_ALL = 1
} PharaohRedrawRequest;

typedef struct PharaohViewState {
    bool layout_edit_active;
    PharaohRedrawRequest redraw_request;
} PharaohViewState;

/* Original address 0x004D6070. */
void pharaoh_view_invalidate_if_editing(PharaohViewState *state);

#endif
