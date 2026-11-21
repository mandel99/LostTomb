#include "pharaoh/ui/economy_actions.h"

#include <limits.h>

uint32_t pharaoh_economy_validate_trade_mode(PharaohEconomyState *state,
    uint32_t resource)
{
    uint32_t eax = state->current_player;
    uint32_t index = eax * 0x24ecu + resource;
    uint32_t result;
    uint16_t mode = state->players[eax].trade_state[resource];
    if (mode == 1u || mode == 3u) {
        result = state->can_export(state->user_data, (int32_t)resource);
        if (result == 0u) {
            eax = state->current_player;
            state->players[eax].trade_state[resource] = (uint16_t)result;
            return (result & 0xffffff00u) | 1u;
        }
        eax = state->current_player;
    }
    index = eax * 0x24ecu + resource;
    eax = (index & 0xffff0000u) |
        state->players[eax].trade_state[resource];
    mode = (uint16_t)eax;
    if (mode == 2u || mode == 4u) {
        eax = state->can_import(state->user_data, (int32_t)resource);
        if (eax == 0u) {
            const uint32_t player = state->current_player;
            index = player * 0x24ecu + resource;
            state->players[player].trade_state[resource] = 0;
            return (index & 0xffffff00u) | 1u;
        }
    }
    return eax & 0xffffff00u;
}

void pharaoh_economy_trade_filter(PharaohEconomyState *state)
{
    PharaohEconomyPlayer *player = &state->players[state->current_player];
    state->redraw_requested = 1;
    player->trade_filter = player->trade_filter == 0u;
}

void pharaoh_economy_trade_back(PharaohEconomyState *state)
{
    state->enter_trade_parent(state->user_data);
    state->redraw_requested = 1;
}

void pharaoh_economy_trade_select(PharaohEconomyState *state)
{
    PharaohEconomyPlayer *player = &state->players[state->current_player];
    const uint32_t index = player->trade_first + state->button_parameter;
    const uint32_t resource = state->trade_resources[index];
    state->selected_trade_resource = resource;
    (void)pharaoh_economy_validate_trade_mode(state, resource);
    state->current_screen = 40;
    state->redraw_requested = 1;
}

void pharaoh_economy_trade_direction(PharaohEconomyState *state)
{
    static const uint32_t next[6][2] = {
        {2, 5}, {4, 3}, {0, 4}, {5, 1}, {1, 2}, {3, 0}
    };
    if (state->trade_direction <= 5u && state->button_parameter <= 1u)
        state->trade_direction = next[state->trade_direction]
            [state->button_parameter];
    state->selection_marker = UINT_MAX;
    state->redraw_requested = 1;
}

void pharaoh_economy_tax_rate(PharaohEconomyState *state)
{
    PharaohEconomyPlayer *player = &state->players[state->current_player];
    if (state->button_parameter == 1u)
        player->tax_rate = (int32_t)((uint32_t)player->tax_rate - 1u);
    else if (state->button_parameter == 0u)
        player->tax_rate = (int32_t)((uint32_t)player->tax_rate + 1u);
    if (player->tax_rate < 0) player->tax_rate = 0;
    else if (player->tax_rate > 25) player->tax_rate = 25;
    state->refresh_tax_projection(state->user_data);
    state->refresh_finance_totals(state->user_data);
    state->refresh_finance_allowance(state->user_data);
    state->redraw_requested = 1;
}

void pharaoh_economy_trade_help(PharaohEconomyState *state)
{
    state->show_message(state->user_data, 46, 1, 0);
    state->redraw_requested = 1;
}

void pharaoh_economy_trade_limit(PharaohEconomyState *state)
{
    uint32_t player = state->current_player;
    uint32_t resource = state->selected_trade_resource;
    uint16_t *field = &state->players[player].trade_limit[resource];
    int32_t price;
    if (state->button_parameter == 1u) --*field;
    else if (state->button_parameter == 0u) ++*field;
    price = (int32_t)(int16_t)*field * 100;
    price = state->clamp_trade_limit(state->user_data, resource, price);
    player = state->current_player;
    price /= 100;
    resource = state->selected_trade_resource;
    state->players[player].trade_limit[resource] = (uint16_t)price;
}

void pharaoh_economy_donate(PharaohEconomyState *state)
{
    PharaohEconomyPlayer *player = &state->players[state->current_player];
    player->donated_total = (int32_t)((uint32_t)player->donated_total +
        (uint32_t)player->donation_amount);
    player->finance_total = (int32_t)((uint32_t)player->finance_total +
        (uint32_t)player->donation_amount);
    player->treasury = (int32_t)((uint32_t)player->treasury -
        (uint32_t)player->donation_amount);
    state->refresh_finance_totals(state->user_data);
    state->refresh_finance_allowance(state->user_data);
    state->current_screen = 7;
    state->redraw_requested = 1;
}

void pharaoh_economy_donation_amount(PharaohEconomyState *state)
{
    PharaohEconomyPlayer *player = &state->players[state->current_player];
    switch (state->button_parameter) {
    case 0: player->donation_amount = 0; break;
    case 1: player->donation_amount = 500; break;
    case 2: player->donation_amount = 2000; break;
    case 3: player->donation_amount = 5000; break;
    case 4: player->donation_amount = 1000000; break;
    default: break;
    }
    if (player->donation_amount > player->treasury)
        player->donation_amount = player->treasury;
    if (player->donation_amount < 0) player->donation_amount = 0;
    state->redraw_requested = 1;
}

void pharaoh_economy_resource_amount_up(PharaohEconomyState *state)
{
    PharaohEconomyPlayer *player = &state->players[state->current_player];
    const uint32_t resource = player->selected_resource;
    const uint32_t amount = player->selected_amount + 1u;
    int32_t maximum = state->resource_ceiling[resource] -
        state->resource_floor[resource];
    player->selected_amount = amount;
    if (player->stock[resource] / 100 < maximum)
        maximum = player->stock[resource] / 100;
    player->selected_amount = (int32_t)amount < maximum ?
        amount : (uint32_t)maximum;
    state->redraw_requested = 1;
}

void pharaoh_economy_resource_amount_down(PharaohEconomyState *state)
{
    PharaohEconomyPlayer *player = &state->players[state->current_player];
    if (state->amount_down_enabled != 0u &&
        (int32_t)player->selected_amount > 0) {
        --player->selected_amount;
    }
    state->redraw_requested = 1;
}
