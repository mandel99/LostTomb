#include "pharaoh/ui/window_state_actions.h"
#include "pharaoh/ui/generic_button_focus.h"
#include <stddef.h>

/* Native004CA980: adopted complete existing C leaf. */
void pharaoh_tooltip_invalidate(int32_t current_tick,
    int32_t *tooltip_id, int32_t *tooltip_tick)
{
    *tooltip_id = 0;
    *tooltip_tick = current_tick;
}

void pharaoh_menu_bar_clear(const PharaohMenuBarState *c)
{
    *c->hovered_item=0;
    *c->selected_header=0;
    *c->active_header=NULL;
    *c->refresh_mode=1;
    *c->redraw_requested=1;
}

void pharaoh_menu_bar_close(const PharaohMenuBarState *c)
{
    *c->current_window=(uint8_t)*c->return_window;
    *c->flag_de1=2;
    *c->flag_de0=1;
    *c->hovered_item=0;
    *c->selected_header=0;
    *c->active_header=NULL;
    *c->refresh_mode=1;
    *c->redraw_requested=1;
}

int32_t pharaoh_menu_bar_select(const PharaohMenuBarState *c,
    const PharaohMenuBarFocusRecord *headers)
{
    if(*c->left_pressed==0 || *c->hovered_header<1) return 0;
    *c->hovered_item=0;
    *c->return_window=*c->current_window;
    *c->current_window=3;
    const uint32_t offset_bits=(*c->selected_header-1u)*16u;
    const int64_t offset=offset_bits<=INT32_MAX?(int64_t)offset_bits:
        (int64_t)offset_bits-INT64_C(0x100000000);
    *c->active_header=headers+(ptrdiff_t)(offset/16);
    *c->refresh_mode=1;
    *c->redraw_requested=1;
    return 1;
}

void pharaoh_city_ratings_store_culture_lowest(
    int32_t *culture_explanation, int32_t culture_lowest)
{
    *culture_explanation = culture_lowest;
}
