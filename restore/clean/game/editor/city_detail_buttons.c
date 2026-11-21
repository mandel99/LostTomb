#include "pharaoh/editor/city_detail_buttons.h"

#include <stddef.h>

enum {
    CITY_KIND = 0,
    CITY_IN_USE = 1,
    CITY_TYPE = 19,
    CITY_NAME = 24,
    CITY_TEXT = 25,
    CITY_ROUTE = 26,
    CITY_VALUE = 28,
    CITY_EDIT_STATE = 51,
    CITY_EDITING = 10,
    RESOURCE_FIRST = 1,
    RESOURCE_END = 36,
    WINDOW_RESOURCE_SELECTION = 0x24
};

_Static_assert(sizeof(PharaohEditorCityRecord) == 98,
    "native editor city record size");

static PharaohEditorCityRecord *selected_city(
    PharaohEditorCityDetailState *state)
{
    return &state->cities[state->selected_city];
}

static int32_t begin_edit(PharaohEditorCityDetailState *state,
    PharaohEditorCityRecord **city)
{
    *city = selected_city(state);
    if ((*city)->bytes[CITY_IN_USE] == 0) return 0;
    (*city)->bytes[CITY_EDIT_STATE] = CITY_EDITING;
    state->selection_active = 0;
    return 1;
}

static void open_value_request(PharaohEditorCityDetailState *state,
    int32_t x, int32_t y, int32_t width, int32_t text_id,
    uint8_t *target)
{
    const PharaohEditorCityValueRequest request = {
        x, y, width, text_id, target
    };
    state->open_value(state->user_data, &request);
    state->redraw_requested = 1;
}

void pharaoh_editor_city_open_name(PharaohEditorCityDetailState *state)
{
    PharaohEditorCityRecord *city;
    if (!begin_edit(state, &city)) return;
    open_value_request(state, 443, state->cursor_y - 314, 7, 39,
        city->bytes + CITY_NAME);
}

void pharaoh_editor_city_open_text(PharaohEditorCityDetailState *state)
{
    PharaohEditorCityRecord *city;
    if (!begin_edit(state, &city)) return;
    open_value_request(state, 20, state->cursor_y - 534, 66,
        state->alternate_text != 0 ? 21 : 195, city->bytes + CITY_TEXT);
}

void pharaoh_editor_city_cycle_type(PharaohEditorCityDetailState *state)
{
    PharaohEditorCityRecord *city;
    if (!begin_edit(state, &city)) return;
    city->bytes[CITY_TYPE] = (uint8_t)(city->bytes[CITY_TYPE] + 1u);
    if (city->bytes[CITY_TYPE] >= 4u) city->bytes[CITY_TYPE] = 0;
    state->redraw_requested = 1;
}

void pharaoh_editor_city_open_alt_text(PharaohEditorCityDetailState *state)
{
    PharaohEditorCityRecord *city;
    if (!begin_edit(state, &city)) return;
    open_value_request(state, 373, state->cursor_y - 534, 17, 196,
        city->bytes + CITY_TEXT);
}

void pharaoh_editor_city_open_route(PharaohEditorCityDetailState *state)
{
    PharaohEditorCityRecord *city;
    if (!begin_edit(state, &city)) return;
    state->open_byte(state->user_data, 453, state->cursor_y - 314,
        city->bytes + CITY_ROUTE);
    state->redraw_requested = 1;
}

void pharaoh_editor_city_open_value(PharaohEditorCityDetailState *state)
{
    PharaohEditorCityRecord *city;
    if (!begin_edit(state, &city)) return;
    state->open_int16(state->user_data, 653, state->cursor_y - 314,
        (int16_t *)(void *)(city->bytes + CITY_VALUE));
    state->redraw_requested = 1;
}

void pharaoh_editor_city_normalize_permissions(
    PharaohEditorCityDetailState *state)
{
    for (int32_t city_index = 0;
         city_index < PHARAOH_EDITOR_CITY_COUNT; ++city_index) {
        PharaohEditorCityRecord *city = &state->cities[city_index];
        if (city->bytes[CITY_IN_USE] == 0 ||
            city->bytes[CITY_KIND] != 1) continue;

        int32_t total = 0;
        for (int32_t resource = RESOURCE_FIRST;
             resource < RESOURCE_END; ++resource) {
            total += state->trade_amount(state->user_data, city_index,
                resource);
        }
        if (total == 0) {
            for (int32_t resource = RESOURCE_FIRST;
                 resource < RESOURCE_END; ++resource) {
                state->permission_set(state->user_data, city_index,
                    resource, 2);
            }
        }
        for (int32_t resource = RESOURCE_FIRST;
             resource < RESOURCE_END; ++resource) {
            if (state->city_sells(state->user_data, city_index, resource) == 0 &&
                state->city_buys(state->user_data, city_index, resource) == 0) {
                state->permission_set(state->user_data, city_index,
                    resource, 0);
            }
        }
    }
}

void pharaoh_editor_city_show_resources(PharaohEditorCityDetailState *state)
{
    int32_t count = 0;
    state->current_window = WINDOW_RESOURCE_SELECTION;
    pharaoh_editor_city_normalize_permissions(state);
    for (int32_t resource = RESOURCE_FIRST;
         resource < RESOURCE_END; ++resource) {
        if (state->city_sells(state->user_data, state->selected_city,
                resource) != 0 ||
            state->city_buys(state->user_data, state->selected_city,
                resource) != 0) {
            state->resources[count++] = (uint32_t)resource;
        }
    }
    while (count < 16) state->resources[count++] = UINT32_MAX;
    state->redraw_requested = 1;
}
