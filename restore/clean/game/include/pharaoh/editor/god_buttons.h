#ifndef PHARAOH_EDITOR_GOD_BUTTONS_H
#define PHARAOH_EDITOR_GOD_BUTTONS_H

#include <stdint.h>

typedef struct PharaohEditorGodState {
    int32_t button_parameter;
    uint16_t *god_status;
    uint16_t *temple_complex;
    uint8_t *redraw_requested;
} PharaohEditorGodState;

/* Complete typed translations of native callback entries 0x00409150 and
   0x004091B0. WORD wrap, signed comparison and repeated reload semantics are
   retained from the pinned EXE. */
void pharaoh_editor_toggle_temple_complex(PharaohEditorGodState *state);
void pharaoh_editor_next_god_status(PharaohEditorGodState *state);

#endif
