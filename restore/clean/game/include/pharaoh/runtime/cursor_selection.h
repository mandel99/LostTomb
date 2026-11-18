#ifndef PHARAOH_RUNTIME_CURSOR_SELECTION_H
#define PHARAOH_RUNTIME_CURSOR_SELECTION_H

#include <stdint.h>

#include "pharaoh/runtime/application_window.h"

typedef enum PharaohCursorSelector {
    PHARAOH_CURSOR_SELECTOR_SLOT_00 = 0x00,
    PHARAOH_CURSOR_SELECTOR_SLOT_01 = 0x01,
    PHARAOH_CURSOR_SELECTOR_SLOT_02 = 0x02,
    PHARAOH_CURSOR_SELECTOR_SLOT_03 = 0x03,
    PHARAOH_CURSOR_SELECTOR_SLOT_04 = 0x04,
    PHARAOH_CURSOR_SELECTOR_SLOT_05 = 0x05,
    PHARAOH_CURSOR_SELECTOR_SLOT_06 = 0x06,
    PHARAOH_CURSOR_SELECTOR_SLOT_07 = 0x07,
    PHARAOH_CURSOR_SELECTOR_SLOT_08 = 0x08,
    PHARAOH_CURSOR_SELECTOR_SLOT_09 = 0x09,
    PHARAOH_CURSOR_SELECTOR_SLOT_0A = 0x0a,
    PHARAOH_CURSOR_SELECTOR_SLOT_0B = 0x0b,
    PHARAOH_CURSOR_SELECTOR_SLOT_0C = 0x0c,
    PHARAOH_CURSOR_SELECTOR_SLOT_0D = 0x0d,
    PHARAOH_CURSOR_SELECTOR_SLOT_0E = 0x0e,
    PHARAOH_CURSOR_SELECTOR_SLOT_0F = 0x0f,
    PHARAOH_CURSOR_SELECTOR_SLOT_10 = 0x10,
    PHARAOH_CURSOR_SELECTOR_SLOT_11 = 0x11,
    PHARAOH_CURSOR_SELECTOR_SLOT_12 = 0x12,
    PHARAOH_CURSOR_SELECTOR_COUNT = 0x13
} PharaohCursorSelector;

typedef struct PharaohCursorSelectionOperations {
    void *context;
    void *(*set_cursor)(void *context, void *cursor_handle);
} PharaohCursorSelectionOperations;

/* Native entry 0x004CF460. Selector values map to named handles loaded by
   pharaoh_application_window_create; values outside 0x00..0x12 select default. */
void pharaoh_cursor_select(
    PharaohApplicationWindowState *state,
    const PharaohCursorSelectionOperations *operations,
    uint32_t selector);

#endif
