#include "pharaoh/editor/permission_buttons.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohEditorPermissionRecord) == 98,
    "native editor permission record stride");
_Static_assert(offsetof(PharaohEditorPermissionRecord, primary) == 0x1e,
    "native primary permission offset");
_Static_assert(offsetof(PharaohEditorPermissionRecord, secondary) == 0x34,
    "native secondary permission offset");
_Static_assert(offsetof(PharaohEditorPermissionRecord, permission_state) ==
    0x3e, "native permission state offset");

static const uint8_t restricted_primary_remap[32] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0,
    1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0
};

static int32_t unused_or_current(const uint8_t *items, int32_t slots,
    int32_t slot, int32_t value)
{
    if (items[slot] == (uint8_t)value) return 1;
    for (int32_t i = 0; i < slots; ++i) {
        if (items[i] == (uint8_t)value) return 0;
    }
    return 1;
}

void pharaoh_editor_open_restricted_primary_permission(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations)
{
    PharaohEditorPermissionRecord *const record =
        &state->records[state->current_record];
    if (record->enabled == 0) return;
    const int32_t slot = state->button_parameter;
    *state->selected_slot = slot;
    record->selection_marker = 10;
    *state->selection_mode = 0;
    state->select_items[0] = 0;
    int32_t count = 1;
    for (uint32_t value = 0; value < 36; ++value) {
        const uint32_t remap_index = value - 1u;
        if (remap_index > 31u || restricted_primary_remap[remap_index] != 0)
            continue;
        if (unused_or_current(record->primary, 14, slot, (int32_t)value))
            state->select_items[count++] = (int32_t)value;
    }
    operations->open_list(operations->user_data, 200, 150, count, 23,
        &record->primary[slot], PHARAOH_PERMISSION_COMPLETION_NONE,
        count / 2 + 1);
}

static void open_permission(PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations, int32_t secondary)
{
    PharaohEditorPermissionRecord *const record =
        &state->records[state->current_record];
    if (record->enabled == 0) return;
    const int32_t slot = state->button_parameter;
    uint8_t *const items = secondary != 0 ? record->secondary :
        record->primary;
    const int32_t slots = secondary != 0 ? 8 : 14;
    *state->selected_slot = slot;
    state->select_items[0] = 0;
    int32_t count = 1;
    for (int32_t value = 1; value < 36; ++value) {
        if (value == 27) continue;
        if (unused_or_current(items, slots, slot, value))
            state->select_items[count++] = value;
    }
    record->selection_marker = 10;
    *state->selection_mode = 0;
    *state->previous_choice = items[slot];
    operations->open_list(operations->user_data, 200, 0, count, 23,
        &items[slot], secondary != 0 ?
            PHARAOH_PERMISSION_COMPLETION_SECONDARY :
            PHARAOH_PERMISSION_COMPLETION_PRIMARY, 18);
}

void pharaoh_editor_open_primary_permission(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations)
{
    open_permission(state, operations, 0);
}

void pharaoh_editor_open_secondary_permission(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations)
{
    open_permission(state, operations, 1);
}

void pharaoh_editor_permission_set(PharaohEditorPermissionState *state,
    int32_t record, int32_t kind, int32_t value)
{
    PharaohEditorPermissionRecord *const item = &state->records[record];
    if (item->type != 1 || item->permission_state_enabled == 0 || value < 0)
        return;
    item->permission_state[kind] = (uint8_t)value;
}

static int32_t find_assignment(
    const PharaohEditorPermissionOperations *operations,
    int32_t secondary, int32_t record, int32_t kind)
{
    return secondary != 0 ?
        operations->find_secondary_assignment(operations->user_data,
            record, kind) :
        operations->find_primary_assignment(operations->user_data,
            record, kind);
}

void pharaoh_editor_permission_update(PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations,
    int32_t secondary)
{
    PharaohEditorPermissionRecord *const record =
        &state->records[state->current_record];
    const int32_t slot = *state->selected_slot;
    const int32_t current = secondary != 0 ? record->secondary[slot] :
        record->primary[slot];
    uint32_t old = *state->previous_choice;
    if (old == 0) {
        if (operations->has_special_assignment(operations->user_data,
                state->current_record, current) != 0) return;
        if (find_assignment(operations, secondary, state->current_record,
                current) != 0) return;
        pharaoh_editor_permission_set(state, state->current_record,
            current, 2);
        return;
    }
    if (current == (int32_t)old) return;
    if (find_assignment(operations, secondary, state->current_record,
            (int32_t)old) == 0) {
        old = *state->previous_choice;
        pharaoh_editor_permission_set(state, state->current_record,
            (int32_t)old, 0);
    }
    if (operations->has_special_assignment(operations->user_data,
            state->current_record, current) != 0) return;
    if (find_assignment(operations, secondary, state->current_record,
            current) == 0) {
        pharaoh_editor_permission_set(state, state->current_record,
            current, 2);
    }
}

void pharaoh_editor_primary_permission_selected(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations)
{
    pharaoh_editor_permission_update(state, operations, 0);
}

void pharaoh_editor_secondary_permission_selected(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations)
{
    pharaoh_editor_permission_update(state, operations, 1);
}

static void reject_permission(PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations,
    int32_t kind, int32_t message)
{
    state->allowed_buildings[kind] = 0;
    operations->show_popup(operations->user_data, message);
    *state->redraw_requested = 1;
}

void pharaoh_editor_toggle_building_permission(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations)
{
    const int32_t kind =
        state->building_kind_by_button[state->button_parameter];
    if (kind == 22) {
        if (operations->requirement_available(operations->user_data, 17) ==
            0) {
            reject_permission(state, operations, 22, 48);
            return;
        }
    } else if (kind == 14) {
        if (operations->requirement_available(operations->user_data, 15) ==
            0) {
            reject_permission(state, operations, 14, 50);
            return;
        }
    } else if (kind == 16 || kind == 17) {
        if (operations->requirement_available(operations->user_data, 23) ==
            0) {
            reject_permission(state, operations, kind, 52);
            return;
        }
    } else if (kind == 41) {
        if (operations->requirement_available(operations->user_data, 29) ==
            0) {
            reject_permission(state, operations, 41, 62);
            return;
        }
    } else if (kind == 42) {
        if (operations->requirement_available(operations->user_data, 20) ==
            0) {
            reject_permission(state, operations, 42, 66);
            return;
        }
    } else if (kind >= 37 && kind <= 40) {
        if (state->allowed_buildings[36] == 0) {
            reject_permission(state, operations, kind, 70);
            return;
        }
        if (kind == 37 && state->allowed_buildings[41] == 0 &&
            operations->requirement_available(operations->user_data, 10) ==
                0) {
            reject_permission(state, operations, 37, 84);
            return;
        }
        if (kind == 39 && state->allowed_buildings[42] == 0 &&
            operations->requirement_available(operations->user_data, 28) ==
                0) {
            reject_permission(state, operations, 39, 86);
            return;
        }
    }
    *state->redraw_requested = 1;
    state->allowed_buildings[kind] =
        (uint16_t)(state->allowed_buildings[kind] == 0);
}
