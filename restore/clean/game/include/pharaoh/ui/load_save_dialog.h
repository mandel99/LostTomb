#ifndef PHARAOH_UI_LOAD_SAVE_DIALOG_H
#define PHARAOH_UI_LOAD_SAVE_DIALOG_H

#include "pharaoh/runtime/file_list.h"

#include <stdint.h>

/* Complete typed C translations of native 0x00402980, 0x004029D0 and
   0x00532830.  The selection projection below is the bounds/index portion
   of local native callback 0x00402A80; its text-input reset remains host-owned. */
int32_t pharaoh_load_dialog_scroll(int32_t direction, int32_t file_count,
    int32_t *first_visible, uint8_t *error_countdown);
int32_t pharaoh_load_dialog_drag_scrollbar(int32_t press_active,
    int32_t mouse_x, int32_t mouse_y, int32_t origin_x, int32_t origin_y,
    int32_t file_count, int32_t *first_visible);
int32_t pharaoh_load_dialog_select_visible(const PharaohFileList *files,
    int32_t first_visible, int32_t visible_row);
int32_t pharaoh_file_dialog_scroll_thumb(int32_t alternate_mode,
    int32_t file_count, int32_t first_visible, int32_t *x, int32_t *y);

#endif
