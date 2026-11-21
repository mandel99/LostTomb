#include "pharaoh/editor/popup_actions.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohEditorEmpireCity) == 106 &&
    offsetof(PharaohEditorEmpireCity, route_id) == 0 &&
    offsetof(PharaohEditorEmpireCity, state) == 1 &&
    offsetof(PharaohEditorEmpireCity, cost) == 0x4a,
    "native editor empire-city layout");

void pharaoh_editor_confirm_city_payment(PharaohEditorPopupState *state)
{
    if (state->family_exists != 1u) return;
    uint32_t selected = state->selected_city;
    const int32_t amount = state->cities[selected].cost;
    state->apply_finance(state->user_data, (int32_t)state->selected_player,
        amount);
    selected = state->selected_city;
    state->cities[selected].state = 1;
    state->refresh_finance(state->user_data);
    state->refresh_city(state->user_data);
    state->popup_result = 0x26;
    state->redraw_requested = 1;
}

void pharaoh_editor_request_city_payment(PharaohEditorPopupState *state)
{
    const PharaohEditorEmpireCity *city =
        &state->cities[state->selected_city];
    if (city->state >= 50u) return;
    if (state->get_player_funds(state->user_data,
            (int32_t)state->selected_player) <= -5000) {
        state->show_dialog(state->user_data, 0x77,
            PHARAOH_EDITOR_DIALOG_ACKNOWLEDGE, 0);
        return;
    }
    state->show_dialog(state->user_data,
        state->route_is_active(state->user_data, city->route_id) == 1u ? 2 : 4,
        PHARAOH_EDITOR_DIALOG_CONFIRM_CITY_PAYMENT, 2);
}

static void open_selection(PharaohEditorPopupState *state,
    const PharaohSelectionPopupRequest *request)
{
    state->open_selection(state->user_data, request);
    state->redraw_requested = 1;
}

void pharaoh_editor_open_word_choice(PharaohEditorPopupState *state)
{
    const PharaohSelectionPopupRequest request = {
        20, 40, 11, 32, 0, NULL, &state->word_choice, NULL,
        0, 2, 0, 0, 0, -1, 0
    };
    open_selection(state, &request);
}

void pharaoh_editor_open_alternate_choice(PharaohEditorPopupState *state)
{
    const int32_t alternate = state->alternate_mode != 0u;
    state->selection_active = 1;
    const PharaohSelectionPopupRequest request = {
        0, 0, alternate ? 62 : 63, 151, alternate ? 63 : 0,
        NULL, NULL, &state->alternate_choice, 0, 4, 0, 0, 0, 22, 0
    };
    open_selection(state, &request);
}

void pharaoh_editor_open_category_choice(PharaohEditorPopupState *state)
{
    const PharaohSelectionPopupRequest request = {
        0, 0, 31, 152, 0, NULL, NULL, &state->category_choice,
        0, 4, 0, 0, 0, 16, 0
    };
    open_selection(state, &request);
}
