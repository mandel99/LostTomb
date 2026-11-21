#include "pharaoh/editor/city_route_buttons.h"

enum {
    ROUTE_LIMIT = 20,
    ROUTE_FIRST_EDITABLE = 1,
    ROUTE_INPUT_X = 453,
    ROUTE_INPUT_Y_OFFSET = 314,
    SCREEN_ROUTE_EDITOR = 14,
    PERMISSION_STATE_MAX = 3
};

void pharaoh_editor_route_open_selection(PharaohEditorCityRouteState *state)
{
    state->open_int32(state->user_data, ROUTE_INPUT_X,
        state->cursor_y - ROUTE_INPUT_Y_OFFSET, &state->selected_route,
        PHARAOH_EDITOR_ROUTE_SELECT);
}

void pharaoh_editor_route_open_spacing(PharaohEditorCityRouteState *state)
{
    state->open_uint8(state->user_data, ROUTE_INPUT_X,
        state->cursor_y - ROUTE_INPUT_Y_OFFSET,
        &state->routes[state->selected_route].spacing,
        PHARAOH_EDITOR_ROUTE_REBUILD);
}

void pharaoh_editor_route_remove(PharaohEditorCityRouteState *state)
{
    PharaohEditorRoute *route = &state->routes[state->selected_route];
    route->point_count = 0;
    route->sample_count = 0;
    if (state->selected_route > ROUTE_FIRST_EDITABLE) {
        --state->selected_route;
    }
    state->redraw_requested = 1;
}

void pharaoh_editor_route_toggle_kind(PharaohEditorCityRouteState *state)
{
    PharaohEditorRoute *route = &state->routes[state->selected_route];
    route->kind = (uint8_t)(1u + (route->kind == 1u));
    state->route_edit_active = 0;
    state->redraw_requested = 1;
}

void pharaoh_editor_route_selection_completed(
    PharaohEditorCityRouteState *state)
{
    if (state->selected_route < ROUTE_FIRST_EDITABLE ||
        state->selected_route >= ROUTE_LIMIT) {
        state->selected_route = ROUTE_FIRST_EDITABLE;
        return;
    }
    PharaohEditorRoute *route = &state->routes[state->selected_route];
    if (route->point_count <= 1u) {
        route->enabled = 0;
    }
    if (route->enabled == 0u) {
        route->point_count = 0;
        route->sample_count = 0;
        state->current_screen = SCREEN_ROUTE_EDITOR;
    }
}

void pharaoh_editor_route_select_mode_image(
    PharaohEditorCityRouteState *state)
{
    state->redraw_requested = 1;
    state->current_building_kind = state->editor_mode;
    const int32_t index = state->editor_mode >= 0 && state->editor_mode <= 3
        ? state->editor_mode : 4;
    state->route_edit_active = 0;
    state->selected_image = state->mode_images[index];
}

void pharaoh_editor_route_cycle_permission(
    PharaohEditorCityRouteState *state)
{
    const int32_t kind = state->permission_kinds[state->editor_mode];
    int32_t value = state->permission_get(state->user_data,
        state->selected_city, kind) + 1;
    if (value > PERMISSION_STATE_MAX) {
        value = 0;
    }
    state->permission_set(state->user_data, state->selected_city, kind, value);
    state->redraw_requested = 1;
}

void pharaoh_editor_city_route_clamp(PharaohEditorCityRouteState *state)
{
    PharaohEditorRouteCityRecord *city = &state->cities[state->selected_city];
    if (city->route_id >= ROUTE_LIMIT) {
        city->route_id = ROUTE_LIMIT - 1;
    }
}
