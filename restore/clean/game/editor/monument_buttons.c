#include "pharaoh/editor/monument_buttons.h"

enum {
    SCREEN_EDITOR_MONUMENTS = 0x4b,
    DISPLAY_MODE_640_X_480 = 1,
    DISPLAY_MODE_800_X_600 = 2,
    MONUMENT_KIND_PYRAMID_FAMILY = 1,
    MONUMENT_KIND_OTHER = 2,
    MONUMENT_SUBKIND_SPHINX = 1,
    MONUMENT_SUBKIND_MAUSOLEUM = 4,
    MONUMENT_SUBKIND_LIGHTHOUSE = 5,
    MONUMENT_ERA_COUNT = 5,
    MONUMENT_NAMES_GROUP = 198,
    MONUMENT_ERAS_GROUP = 311
};

static void list_position(int32_t display_mode, int32_t *x, int32_t *y)
{
    if (display_mode == DISPLAY_MODE_640_X_480) {
        *x = 110;
        *y = 140;
    } else if (display_mode == DISPLAY_MODE_800_X_600) {
        *x = 190;
        *y = 200;
    } else {
        *x = 302;
        *y = 284;
    }
}

static int32_t choice_blocked(const PharaohEditorMonumentDefinition *defs,
    const PharaohEditorMonumentDefinition *definition, int32_t other_id)
{
    const PharaohEditorMonumentDefinition *const other = &defs[other_id];
    if (other->kind != definition->kind) return 0;
    const int32_t one_kind_only =
        other->kind == MONUMENT_KIND_PYRAMID_FAMILY ||
        (other->kind == MONUMENT_KIND_OTHER &&
            other->subkind < MONUMENT_SUBKIND_LIGHTHOUSE);
    if (one_kind_only && other->subkind != definition->subkind) return 1;
    return other->kind == MONUMENT_KIND_OTHER &&
        (other->subkind == MONUMENT_SUBKIND_SPHINX ||
         other->subkind == MONUMENT_SUBKIND_MAUSOLEUM);
}

void pharaoh_editor_open_monuments(PharaohEditorMonumentState *state)
{
    *state->current_screen = SCREEN_EDITOR_MONUMENTS;
    *state->redraw_requested = 1;
}

void pharaoh_editor_select_monument(PharaohEditorMonumentState *state,
    const PharaohEditorMonumentOperations *operations)
{
    const int32_t parameter = state->button_parameter;
    const int32_t other_a = state->monument_definition_ids[
        parameter == 0 ? 1 : 0];
    const int32_t other_b = state->monument_definition_ids[
        parameter == 2 ? 1 : 2];
    int32_t count = 1;
    state->select_items[0] = 0;
    for (int32_t id = 1; id < state->definition_count; ++id) {
        const PharaohEditorMonumentDefinition *const definition =
            &state->definitions[id];
        if (operations->goods_available(operations->user_data, id) == 0 ||
            operations->definition_allowed(operations->user_data,
                state->animation_set, definition) == 0) {
            continue;
        }
        const int32_t check_other_slots = !(
            definition->kind == MONUMENT_KIND_OTHER &&
            definition->subkind >= MONUMENT_SUBKIND_LIGHTHOUSE);
        if (check_other_slots &&
            (choice_blocked(state->definitions, definition, other_a) ||
             choice_blocked(state->definitions, definition, other_b))) {
            continue;
        }
        state->select_items[count++] = id;
    }

    int32_t x;
    int32_t y;
    list_position(state->display_mode, &x, &y);
    operations->open_list(operations->user_data, x, y, count,
        MONUMENT_NAMES_GROUP,
        &state->monument_definition_ids[parameter],
        PHARAOH_EDITOR_TARGET_INT16, 1, 1, (count + 1) / 2);
}

void pharaoh_editor_set_monument_era(PharaohEditorMonumentState *state,
    const PharaohEditorMonumentOperations *operations)
{
    int32_t x;
    int32_t y;
    list_position(state->display_mode, &x, &y);
    operations->open_list(operations->user_data, x, y,
        MONUMENT_ERA_COUNT, MONUMENT_ERAS_GROUP, &state->animation_set,
        PHARAOH_EDITOR_TARGET_INT8, 0, 0, 3);
}

void pharaoh_editor_edit_burial_provision(
    PharaohEditorMonumentState *state,
    const PharaohEditorMonumentOperations *operations)
{
    const int32_t monument = state->button_parameter;
    if ((uint8_t)operations->resource_available(
            operations->user_data, monument) == 0) {
        operations->show_popup(operations->user_data, 28);
        *state->redraw_requested = 1;
        return;
    }
    if (operations->provision_kind_count(
            operations->user_data, monument) >= 5) {
        operations->show_popup(operations->user_data, 26);
        *state->redraw_requested = 1;
        return;
    }
    operations->open_numeric_input(operations->user_data, 300, 200, 4,
        &state->provisions_needed[monument]);
    *state->redraw_requested = 1;
}
