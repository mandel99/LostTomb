#include "pharaoh/military/legion_orders.h"

void pharaoh_formation_change_morale(PharaohLegionOrderState *state,
    int32_t formation_id, int16_t delta)
{
    PharaohLegionOrderFormation *formation;
    int32_t maximum;
    uint16_t morale;

    if (formation_id == 0)
        return;

    formation = &state->formations[(uint32_t)formation_id];
    if (formation->kind == 13)
        maximum = formation->leader_id != 0 ? 100 : 80;
    else if (formation->kind == 57)
        maximum = 100;
    else if (formation->kind == 12 || formation->kind == 11)
        maximum = formation->leader_id != 0 ? 80 : 60;
    else
        maximum = formation->subtype == 13 ? 80 : 70;

    morale = (uint16_t)((uint16_t)formation->morale + (uint16_t)delta);
    formation->morale = (int16_t)morale;
    if (formation->morale > maximum)
        formation->morale = (int16_t)maximum;
    if (formation->morale < 0)
        formation->morale = 0;
}

void pharaoh_legion_order_button(PharaohLegionOrderState *state)
{
    uint32_t formation_id = state->selected_formation_id;
    uint32_t button_parameter = state->button_parameter;
    uint32_t current_walker_id;
    PharaohLegionOrderFormation *formation = &state->formations[formation_id];
    int16_t order;
    uint32_t index;

    if (formation->availability >= 0 || state->command_locked != 0)
        return;

    if (button_parameter == 3 && formation->order != 6)
        formation->previous_order = formation->order;

    if (formation->kind == 13 &&
        state->walkers[(uint32_t)(int32_t)formation->walkers[0]].action_state == 19 &&
        button_parameter != 1) {
        current_walker_id = state->current_walker_id;
        for (index = 0; index < PHARAOH_LEGION_WALKER_COUNT; ++index) {
            int16_t walker_id = formation->walkers[index];
            PharaohLegionOrderWalker *walker;
            if (walker_id == 0)
                break;
            walker = &state->walkers[(uint32_t)(int32_t)walker_id];
            if (walker->action_state == 4 || formation->disabled != 0 ||
                formation->order_mode > 1)
                continue;
            if (formation->order_mode == 1) {
                pharaoh_formation_change_morale(state, (int32_t)formation_id, 10);
                current_walker_id = state->current_walker_id;
            }
            walker->state = 0;
            walker->route = 0;
            walker->target_x = walker->x;
            walker->target_y = walker->y;
            walker->action_state = 12;
            state->walkers[current_walker_id].animation_frame = 0;
            button_parameter = state->button_parameter;
        }
    }

    switch (button_parameter) {
    case 0:
        formation->order = formation->alternate_order != 0 ? 2 : 1;
        break;
    case 1:
        if (formation->kind == 13) {
            if (formation->field_126 == 0) {
                order = formation->order;
                if (order != 0)
                    formation->resume_order = order;
                formation->order = 0;
            }
        } else {
            formation->order = formation->alternate_order != 0 ? 4 : 3;
        }
        break;
    case 2:
        formation->order = 15;
        break;
    case 3:
        formation->order = 6;
        break;
    default:
        break;
    }

    state->active_formation_id = formation_id;
    order = formation->order;
    formation->task = UINT32_MAX;
    if (order == 6) {
        state->sidebar_update_code = 1;
    } else if (order == 0) {
        state->sidebar_update_code = 0x49;
    } else if (formation->field_126 != 0) {
        state->sidebar_update_code = state->button_parameter == 1 ? 9 : 0x1e;
    } else {
        state->sidebar_update_code = 0x1e;
    }
    state->redraw_requested = 1;
}
