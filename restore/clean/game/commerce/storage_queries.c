#include "pharaoh/commerce/storage_queries.h"

int32_t pharaoh_storage_image_level(int32_t amount)
{
    if (amount == 400) return 3;
    if (amount >= 300 && amount < 400) return 2;
    if (amount >= 200 && amount < 300) return 1;
    return 0;
}

int32_t pharaoh_storageyard_resource_amount(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, int32_t resource)
{
    int32_t slot;
    int32_t total = 0;
    for (slot = 0; slot < 8; ++slot) {
        building_id = buildings[building_id].next_storage_building_id;
        if (building_id < 1) return 0;
        if (buildings[building_id].subtype != 0 &&
            buildings[building_id].subtype == resource) {
            total += buildings[building_id].loads_stored_render;
        }
    }
    return total;
}

void pharaoh_storage_orders_adjust_limit(PharaohStorageLimitState *state)
{
    int32_t visible_rows;
    int32_t resource;
    int32_t limit_index;
    int32_t limit;
    uint8_t policy;
    int16_t *limits;
    bool redraw_first;

    if (*state->orders_scroll == 0) {
        *state->orders_scroll = 1;
    } else {
        visible_rows = pharaoh_storage_orders_visible_rows(
            state->display_mode);
        if (visible_rows < *state->orders_scroll) {
            visible_rows = pharaoh_storage_orders_visible_rows(
                state->display_mode);
            *state->orders_scroll =
                state->storage_resource_count - visible_rows + 1;
        }
    }

    if (state->building_type == 72 || state->building_type == 193) {
        resource = state->storage_resources[
            *state->orders_scroll + state->row - 1];
    } else {
        resource = state->food_resources[state->row];
    }

    if (state->action != 0u && state->action != 1u) {
        *state->redraw_requested = 1;
        return;
    }

    policy = state->accept_modes[
        state->owner * PHARAOH_STORAGE_ACCEPT_MODE_STRIDE + resource];
    limit_index = state->owner * PHARAOH_STORAGE_RESOURCE_SLOTS + resource;
    limits = policy == 0u ? state->policy_zero_limits :
        state->policy_other_limits;
    limit = limits[limit_index];
    redraw_first = policy == 0u;

    if (state->action == 1u) {
        limit -= 800;
        if (limit < 800) {
            limit = 800;
            redraw_first = true;
        }
    } else {
        limit += 800;
        if (limit > 3200) limit = 3200;
    }

    if (redraw_first) {
        *state->redraw_requested = 1;
        limits[limit_index] = (int16_t)limit;
    } else {
        limits[limit_index] = (int16_t)limit;
        *state->redraw_requested = 1;
    }
}

void pharaoh_storage_orders_scroll(uint32_t direction, int32_t display_mode,
    int32_t resource_count, int32_t *scroll, uint8_t *redraw_requested)
{
    int32_t visible_rows;

    if (direction == 0u) {
        --*scroll;
        if (*scroll < 1) {
            *scroll = 1;
            *redraw_requested = 1;
            return;
        }
    } else {
        ++*scroll;
        visible_rows = pharaoh_storage_orders_visible_rows(display_mode);
        if (resource_count < visible_rows - 1 + *scroll) {
            visible_rows = pharaoh_storage_orders_visible_rows(display_mode);
            *scroll = resource_count - visible_rows + 1;
        }
    }
    *redraw_requested = 1;
}

int32_t pharaoh_storage_orders_visible_rows(int32_t display_mode)
{
    if (display_mode == 1) return 15;
    if (display_mode == 2) return 21;
    if (display_mode == 3) return 24;
    return 10;
}

int32_t pharaoh_storageyard_get_space_info(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id)
{
    int32_t step;
    int32_t unfinished = 0;
    int32_t total = 0;

    for (step = 0; step < 8; ++step) {
        building_id = buildings[building_id].next_storage_building_id;
        if (building_id < 1) return 0;
        if (buildings[building_id].subtype == 0) ++unfinished;
        else total += buildings[building_id].storage_link_amount;
    }
    if (unfinished > 0) return 0;
    return total < 3200 ? 2 : 1;
}
