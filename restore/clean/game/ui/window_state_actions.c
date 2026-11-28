#include "pharaoh/core/percentage_scale.h"
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

void pharaoh_build_sidebar_hover_update(int32_t mouse_x, int32_t mouse_y,
    int32_t left, int32_t top, int32_t right, int32_t bottom,
    int32_t *hovered)
{
    *hovered = mouse_x >= left && mouse_x < right &&
        mouse_y >= top && mouse_y < bottom;
}

void pharaoh_top_menu_hover_index(int32_t mouse_x, int32_t mouse_y,
    const int32_t left_edges[3], int32_t *hovered)
{
    int32_t index;
    *hovered = 0;
    if (mouse_y <= 3 || mouse_y >= 18) return;
    for (index = 0; index < 3; ++index) {
        if (mouse_x > left_edges[index] && mouse_x < left_edges[index] + 128)
            *hovered = index + 1;
    }
}

void pharaoh_game_undo_finish_build(uint32_t placement_cursor,
    uint32_t *undo_cursor, int32_t *undo_ticks,
    int32_t *undo_ready, uint8_t *redraw_requested)
{
    *undo_ready = 1;
    *undo_ticks = 500;
    *undo_cursor = placement_cursor;
    *redraw_requested = 1;
}

void pharaoh_popup_window_open(PharaohPopupWindowState *state,
    uint32_t key, void (*callback)(void), uint32_t mode)
{
    state->previous_window = state->current_window;
    state->popup_y = state->mouse_y + 0x50;
    state->popup_x = state->mouse_x + 0x50;
    state->current_window = 0x3f;
    state->popup_flag = 0;
    state->callback = callback;
    state->callback_uses_context = 0;
    state->callback_context = 0;
    state->key = key;
    state->mode = mode;
    state->redraw_requested = 1;
}

void pharaoh_popup_window_open_context(PharaohPopupWindowState *state,
    uint32_t key,void (*callback)(const void *),const void *context,uint32_t mode)
{
    pharaoh_popup_window_open(state,key,0,mode);
    state->context_callback=callback;
    state->callback_context=context;
    state->callback_uses_context=1;
}
void pharaoh_popup_window_invoke_callback(const PharaohPopupWindowState *state)
{
    if (state->callback_uses_context)
        state->context_callback(state->callback_context);
    else state->callback();
}

void pharaoh_building_info_show_orders(
    int32_t *orders_active, uint8_t *redraw_requested)
{
    *orders_active = 1;
    *redraw_requested = 1;
}

void pharaoh_transport_ship_button(PharaohTransportShipButtonState *state)
{
    PharaohWalkerQueryRecord *walker;
    int32_t percentage;

    state->selected_transport_id = state->transport_id;
    if (state->transport_id <= 0 || state->input_locked != 0u) return;
    walker = &state->walkers[state->transport_id];
    percentage = pharaoh_scale_value_to_percent(walker->hit_points,
        walker->maximum_hit_points).value;
    if ((walker->action_state == 11u || walker->action_state == 15u) &&
        percentage <= 15) return;
    if (state->button_parameter == 3u &&
        (walker->action_state == 11u || walker->action_state == 15u ||
         percentage == 100)) return;
    switch (state->button_parameter) {
        case 0:
            walker->action_state = 20;
            state->selected_transport_active = 1;
            state->current_window = 31;
            state->redraw_requested = 1;
            break;
        case 1:
            walker->action_state = 10;
            walker->wait_ticks = 75;
            walker->transport_order_15c = 16;
            state->selected_transport_active = 0;
            state->current_window = 1;
            state->redraw_requested = 1;
            break;
        case 2:
            walker->action_state = (uint8_t)(12u +
                (walker->transport_state_130 != 0u));
            state->selected_transport_active = 1;
            state->current_window = 31;
            state->redraw_requested = 1;
            break;
        case 3:
            state->selected_transport_active = 0;
            state->assign_building(state->callback_context,
                state->transport_id);
            state->current_window = 31;
            state->redraw_requested = 1;
            break;
        default:
            break;
    }
}

void pharaoh_transport_ship_return_button(
    PharaohTransportShipButtonState *state,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int16_t *slot_walkers, int32_t selected_walker_id,
    PharaohBuildingTileResolver resolve_tile, void *resolve_context)
{
    PharaohWalkerQueryRecord *walker;
    int32_t percentage;

    state->selected_transport_id = state->transport_id;
    if (state->transport_id <= 0) return;
    walker = &state->walkers[state->transport_id];
    percentage = pharaoh_scale_value_to_percent(walker->hit_points,
        walker->maximum_hit_points).value;
    if ((walker->action_state == 11u || walker->action_state == 15u) &&
        percentage <= 15) return;
    if (state->button_parameter == 4u) {
        int16_t destination_x;
        int16_t destination_y;
        const int32_t building_id = walker->building_id;

        if (pharaoh_ship_is_docked(state->walkers, buildings,
            state->transport_id)) return;
        /* The native callback reloads buttonParameter after the query. */
        if (state->button_parameter == 4u) {
            walker->action_state = 9;
            state->selected_transport_active = 0;
            resolve_tile(resolve_context, buildings[building_id].x,
                buildings[building_id].y, buildings[building_id].size,
                &destination_x, &destination_y);
            walker->source_x = walker->x;
            walker->source_y = walker->y;
            walker->destination_x = destination_x;
            walker->destination_y = destination_y;
            pharaoh_walker_detach_slot_link(state->walkers, slot_walkers,
                selected_walker_id);
        }
    }
    state->current_window = 1;
    state->redraw_requested = 1;
}
