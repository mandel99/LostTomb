#include "pharaoh/editor/validation.h"

enum {
    MAP_ROW_STRIDE = 228,
    TERRAIN_ROAD = 4,
    WINDOW_EDITOR_VALID = 16
};

static uint32_t terrain_at(const PharaohEditorValidationState *state,
    int32_t x, int32_t y)
{
    const uint32_t index = state->grid_base + (uint32_t)x +
        (uint32_t)y * MAP_ROW_STRIDE;
    return state->terrain[index];
}

bool pharaoh_editor_terrain_allowed(uint32_t terrain)
{
    if ((terrain & 0x3038d022u) != 0u) return false;
    const uint32_t masked = terrain & 0x26351277u;
    if (masked == 0u) return true;
    if ((masked & TERRAIN_ROAD) != 0u) return false;
    return ((~(masked >> 18)) & 1u) != 0u;
}

bool pharaoh_editor_validate_terrain_kinds(
    const PharaohEditorValidationState *state)
{
    for (int32_t i = 0; i < 4; ++i) {
        const PharaohEditorPoint16 point = state->terrain_kind_points16[i];
        if (point.x >= 0 && point.y >= 0 &&
            !state->terrain_kind_valid(state->user_data,
                terrain_at(state, point.x, point.y), 0)) return false;
    }
    for (int32_t i = 0; i < 4; ++i) {
        const PharaohEditorPoint32 point = state->terrain_kind_points32[i];
        if (point.x >= 0 && point.y >= 0 &&
            !state->terrain_kind_valid(state->user_data,
                terrain_at(state, point.x, point.y), 1)) return false;
    }
    return true;
}

static bool validate_roads(const PharaohEditorValidationState *state,
    const PharaohEditorPoint16 *points, int32_t count, bool positive_pair)
{
    for (int32_t i = 0; i < count; ++i) {
        const PharaohEditorPoint16 point = points[i];
        const bool present = positive_pair
            ? point.x > 0 || point.y > 0
            : point.x >= 0 && point.y >= 0;
        if (present &&
            (terrain_at(state, point.x, point.y) & TERRAIN_ROAD) == 0u) {
            return false;
        }
    }
    return true;
}

bool pharaoh_editor_validate_road_points(
    const PharaohEditorValidationState *state)
{
    return validate_roads(state, state->road_points, 8, false);
}

bool pharaoh_editor_validate_allowed_pair(
    const PharaohEditorValidationState *state)
{
    for (int32_t i = 0; i < 2; ++i) {
        const PharaohEditorPoint16 point = state->allowed_pair[i];
        if ((point.x > 0 || point.y > 0) &&
            !pharaoh_editor_terrain_allowed(
                terrain_at(state, point.x, point.y))) return false;
    }
    return true;
}

bool pharaoh_editor_validate_road_pair(
    const PharaohEditorValidationState *state)
{
    return validate_roads(state, state->road_pair, 2, true);
}

bool pharaoh_editor_validate_allowed_points(
    const PharaohEditorValidationState *state)
{
    for (int32_t i = 0; i < 8; ++i) {
        const PharaohEditorPoint16 point = state->allowed_points[i];
        if (point.x >= 0 && point.y >= 0 &&
            !pharaoh_editor_terrain_allowed(
                terrain_at(state, point.x, point.y))) return false;
    }
    return true;
}

bool pharaoh_editor_validate_second_road_points(
    const PharaohEditorValidationState *state)
{
    return validate_roads(state, state->road_points_second, 8, false);
}

static bool reject(PharaohEditorValidationState *state,
    int32_t first, int32_t second)
{
    state->show_message(state->user_data, first, second);
    return false;
}

bool pharaoh_editor_validate_and_commit(PharaohEditorValidationState *state)
{
    if (!pharaoh_editor_validate_terrain_kinds(state))
        return reject(state, 0x6c, 0x6d);
    if (!pharaoh_editor_validate_road_points(state))
        return reject(state, 0x7f, 0x80);
    if (!pharaoh_editor_validate_allowed_pair(state))
        return reject(state, 0x81, 0x82);
    if (!pharaoh_editor_validate_road_pair(state))
        return reject(state, 0x83, 0x84);
    if (!pharaoh_editor_validate_allowed_points(state))
        return reject(state, 0x85, 0x86);
    if (!pharaoh_editor_validate_second_road_points(state))
        return reject(state, 0x87, 0x88);
    if (!state->extra_check(state->user_data))
        return reject(state, 0x79, 0x7a);

    state->commit_valid_map(state->user_data, 1);
    state->input_pending = 0;
    state->current_window = WINDOW_EDITOR_VALID;
    state->return_window = 0;
    state->final_state = 0;
    state->redraw_requested = 1;
    return true;
}

uint32_t pharaoh_editor_terrain_kind_result(
    const PharaohEditorTerrainKindContext *context,
    uint32_t terrain, int8_t mode)
{
    const uint32_t scene = context->scene & 0xffu;
    const uint32_t index = mode != 0 ? scene :
        (uint32_t)((int32_t)context->alternate_index + (int32_t)(scene * 2u));
    const uint32_t allowed = mode != 0 ?
        context->mode_allowed[index * 12u] :
        context->default_allowed[index * 12u];
    uint32_t masked;
    if ((terrain & 0x3038d022u) != 0u) return terrain & 0xffffff00u;
    masked = terrain & 0x26351277u;
    if (allowed != 0u || masked != 0u) {
        if (context->rule_mode == 1u)
            return (masked & 0xffffff00u) | ((masked & allowed) != 0u);
        if ((masked & 4u) != 0u && (allowed & 4u) == 0u)
            return masked & 0xffffff00u;
        if ((masked & 0x40000u) != 0u && (allowed & 0x40000u) == 0u)
            return masked & 0xffffff00u;
    }
    return (masked & 0xffffff00u) | 1u;
}
