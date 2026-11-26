#include "pharaoh/ui/menu_dropdown_input.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohMenuDropdownRow) == 16, "native menu item stride");
_Static_assert(sizeof(PharaohMenuBarFocusRecord) == 16, "native header stride");

static ptrdiff_t native_item_offset(uint32_t index)
{
    const uint32_t bits = index * UINT32_C(16) - UINT32_C(16);
    const int64_t bytes = bits <= INT32_MAX ? (int64_t)bits :
        (int64_t)bits - INT64_C(0x100000000);
    return (ptrdiff_t)(bytes / 16);
}

int32_t pharaoh_menu_dropdown_handle_input(
    const PharaohMenuDropdownInputContext *context,
    const PharaohMenuBarFocusRecord *headers, int32_t count)
{
    const PharaohMenuBarState *const menu = context->menu;
    const uint8_t clicked = *menu->left_pressed;
    *context->keep_open = 0;

    if (clicked != 0) {
        const uint32_t row = *menu->hovered_item;
        if (row == 0) {
            pharaoh_menu_bar_close(menu);
            return 1;
        }
        const uint8_t return_window = (uint8_t)*menu->return_window;
        const PharaohMenuBarFocusRecord *const header = *menu->active_header;
        *menu->flag_de1 = 1;
        *menu->flag_de0 = 1;
        *menu->current_window = return_window;
        const PharaohMenuDropdownHeaderData *const data =
            context->header_data + (header - context->header_storage);
        const ptrdiff_t offset = native_item_offset(row);
        const PharaohMenuDropdownRow *const item = *data->items + offset;
        *context->current_item = item;
        memcpy(context->button_parameter, item->reserved_04 + 8,
            sizeof(*context->button_parameter));
        const PharaohMenuDropdownAction *const action = data->actions + offset;
        action->invoke(action->user_data);
        if (*context->keep_open != 1) {
            pharaoh_menu_bar_clear(menu);
            return 1;
        }
        *menu->redraw_requested = 1;
        *menu->refresh_mode = 1;
        *menu->current_window = 3;
        return 1;
    }

    const int32_t hit = pharaoh_menu_bar_focus(headers, count,
        *context->focus->cursor_x, *context->focus->cursor_y);
    if ((uint32_t)hit != *menu->selected_header && hit != 0) {
        *menu->selected_header = (uint32_t)hit;
        *menu->flag_de1 = 1;
        *menu->flag_de0 = 1;
        const PharaohMenuBarFocusRecord *const header =
            headers + native_item_offset((uint32_t)hit);
        *menu->hovered_item = 0;
        *menu->active_header = header;
        *menu->refresh_mode = 1;
        *menu->redraw_requested = 1;
    }
    const PharaohMenuBarFocusRecord *const header = *menu->active_header;
    const PharaohMenuDropdownHeaderData *const data =
        context->header_data + (header - context->header_storage);
    const int32_t y = header->top;
    const int32_t x = header->left;
    const int32_t item_count = *data->count;
    const PharaohMenuDropdownRow *const items = *data->items;
    *menu->hovered_item = (uint32_t)pharaoh_menu_dropdown_focus(context->focus,
        items, item_count, x, y);
    return 0;
}
