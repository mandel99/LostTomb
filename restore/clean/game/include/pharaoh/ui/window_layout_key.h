#ifndef PHARAOH_UI_WINDOW_LAYOUT_KEY_H
#define PHARAOH_UI_WINDOW_LAYOUT_KEY_H

#include <stdint.h>
#include "pharaoh/ui/bitmap_font.h"
#include "pharaoh/ui/generic_button_input.h"

typedef struct PharaohUiLayoutNode {
    int32_t relative_x;
    int32_t relative_y;
    int32_t values_08[4];
    struct PharaohUiLayoutNode *next;
} PharaohUiLayoutNode;

_Static_assert(sizeof(PharaohUiLayoutNode) == 0x1c,
    "native UI layout node size changed");

typedef struct PharaohUiLayoutEntry {
    PharaohUiLayoutNode *lists[3];
    int32_t buttons_initialized;
} PharaohUiLayoutEntry;

_Static_assert(sizeof(PharaohUiLayoutEntry) == 0x10,
    "three native layout lists and one DWORD flag");

typedef struct PharaohUiLayoutTextContext {
    PharaohTextSelection *selection;
    const PharaohBitmapFont *font; /* native font 0x00F56F8C */
    const uint8_t *glyph_map;
    uint32_t *current_glyph;
    const char **current_text_00e20768;
} PharaohUiLayoutTextContext;

/* Complete native 0x004D5F80, 0x004D5DD0 and 0x004D5E70.
   The catalog must contain the native adjacent-group sentinel entry.
   Buttons are logical C records converted from the 30 native 28-byte rows. */
int32_t pharaoh_ui_layout_group_max_width(
    const PharaohUiLayoutTextContext *context, int32_t group);
void pharaoh_ui_layout_init_column_widths(
    const PharaohUiLayoutTextContext *context, int32_t columns[6]);
void pharaoh_ui_layout_init_label_width(
    const PharaohUiLayoutTextContext *context, PharaohGenericButton buttons[30],
    int32_t *label_columns, int32_t *label_width);

/* Original entry 0x004D5D70. The special layout base is read from native
   state only for window key 7. */
int32_t pharaoh_ui_layout_window_key(int32_t window_key,
    int32_t window_seven_layout_base);

PharaohUiLayoutNode *pharaoh_ui_layout_find(
    const PharaohUiLayoutEntry *entries, int32_t kind, int32_t mode,
    int32_t relative_x, int32_t relative_y);

#endif
