#ifndef PHARAOH_UI_MENU_DROPDOWN_DRAW_H
#define PHARAOH_UI_MENU_DROPDOWN_DRAW_H

#include "pharaoh/ui/window_state_actions.h"
#include "pharaoh/ui/generic_button_focus.h"
#include "pharaoh/ui/panel_frame.h"
#include "pharaoh/ui/bitmap_font.h"

typedef struct PharaohMenuDropdownDrawContext {
    const PharaohMenuBarState *menu;
    /* Actual relocated items/count fields of the active header (+8/+12).
       Native VA tokens in the old raw header must never be executed. */
    const PharaohMenuDropdownRow *const *header_items;
    const int16_t *header_count;
    const PharaohMenuDropdownRow **current_item; /* actual705720 */
    const int32_t *menu_widths; /* six measured selector cells E945D0..E945E4 */
    int16_t *scratch_height,*scratch_width; /* D3B5BC/D3B5BE */
    const PharaohPanelFrameNativeContext *panel;
    const PharaohBitmapFont *normal_font,*highlight_font; /* tags635/638 */
    PharaohBitmapTextDrawState *drawing;
    PharaohTextSelection *selection;
} PharaohMenuDropdownDrawContext;

/* Complete00425430; real bitmap and panel children, shared cursor/outline
   and WORD scratch effects. Requires genuine relocated active-header data. */
void pharaoh_menu_dropdown_draw(const PharaohMenuDropdownDrawContext *);

#endif
