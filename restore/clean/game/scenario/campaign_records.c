#include "pharaoh/scenario/campaign_records.h"

#include <limits.h>

typedef struct WinnerCandidate {
    int32_t value;
    uint32_t index;
    uint8_t tie_breaker;
} WinnerCandidate;

static void update_winner(WinnerCandidate *winner, int32_t value,
    uint8_t tie_breaker, uint32_t index)
{
    if (value > winner->value ||
        (value == winner->value && tie_breaker > winner->tie_breaker)) {
        winner->value = value;
        winner->tie_breaker = tie_breaker;
        winner->index = index;
    }
}

void pharaoh_mission_records_store(PharaohMissionRecordState *state,
    const PharaohMissionRecordOperations *operations)
{
    WinnerCandidate best[3] = {
        { -1, UINT32_MAX, 0 },
        { -1, UINT32_MAX, 0 },
        { -1, UINT32_MAX, 0 }
    };

    *state->stored_personal_savings = state->current_personal_savings;
    for (int32_t index = 1; index < state->monument_count; ++index) {
        const PharaohMissionMonumentRecord *const record =
            &state->monuments[index];
        if (record->active == 1 && record->eligible != 0) {
            const uint32_t slot = record->type == 12 ? 0u :
                record->type == 11 ? 1u : 2u;
            update_winner(&best[slot], record->value,
                record->tie_breaker, (uint32_t)index);
        }
    }
    for (uint32_t slot = 0; slot < 3; ++slot) {
        PharaohMissionWinner *const output = &state->winners[slot];
        output->value = best[slot].value;
        output->tie_breaker = best[slot].tie_breaker;
        output->result_id = best[slot].index == UINT32_MAX ? UINT8_MAX :
            state->monuments[best[slot].index].result_id;
    }

    const char *const text = operations->select_text(operations->user_data,
        0x133, (int32_t)state->settings->mission_id);
    if (text[6] == '1') {
        for (int32_t index = 1; index < state->building_count; ++index) {
            const PharaohMissionBuildingRecord *const building =
                &state->buildings[index];
            if (building->active != 1) continue;
            switch (building->type) {
            case 0xde:
                state->special_positions->type_de_first =
                    building->packed_first;
                state->special_positions->type_de_packed =
                    (uint16_t)(building->packed_low |
                        (uint16_t)((uint32_t)building->packed_high << 13));
                break;
            case 0xe1:
                state->special_positions->type_e1_x =
                    (uint16_t)(building->x - 4u);
                state->special_positions->type_e1_y =
                    (uint16_t)(building->y + 2u);
                break;
            case 0xe4: {
                const int32_t parent = index - building->parent_offset;
                state->special_positions->type_e4_x =
                    state->buildings[parent].x;
                state->special_positions->type_e4_y =
                    state->buildings[parent].y;
                break;
            }
            case 0xe5:
                state->special_positions->type_e5_x =
                    (uint16_t)(building->x - 5u);
                state->special_positions->type_e5_y =
                    (uint16_t)(building->y + 6u);
                break;
            case 0xea:
                state->special_positions->type_ea_x =
                    (uint16_t)(building->x - 6u);
                state->special_positions->type_ea_y =
                    (uint16_t)(building->y + 8u);
                break;
            case 0xeb:
                state->special_positions->type_eb_x =
                    (uint16_t)(building->x + 1u);
                state->special_positions->type_eb_y =
                    (uint16_t)(building->y + 17u);
                break;
            default:
                break;
            }
        }
    }
    operations->save_profile(operations->user_data);
}

static void recreate_monument(const PharaohMissionRecordApplyOperations *ops,
    int32_t type, int32_t x, int32_t y)
{
    const int32_t building_id = ops->recreate_monument(
        ops->user_data, type, x, y);
    ops->complete_monument(ops->user_data, type, building_id);
}

void pharaoh_mission_records_apply(PharaohMissionRecordApplyState *state,
    const PharaohMissionRecordApplyOperations *operations)
{
    enum {
        BUILDING_MAUSOLEUM = 0xde,
        BUILDING_LIBRARY = 0xe1,
        BUILDING_LIGHTHOUSE = 0xe4,
        BUILDING_SMALL_ROYAL_TOMB = 0xe5,
        BUILDING_MEDIUM_ROYAL_TOMB = 0xea,
        BUILDING_LARGE_ROYAL_TOMB = 0xeb
    };

    *state->started = *state->finished;
    const char *const options = operations->select_text(
        operations->user_data, 0x133, state->settings->mission_id);
    state->settings->personal_savings_last_mission =
        options[0] == '1' ? state->started->savings : 0;
    state->started->savings = 0;

    /* Serialized coordinates keep raw WORD bits; native call arguments
       sign-extend those WORDs (the packed DE x coordinate is masked). */
    const PharaohMissionSpecialPositions *const positions =
        state->special_positions;
    if (options[4] == 'A') {
        if ((int16_t)positions->type_de_packed != 0 &&
            positions->type_de_first != 0) {
            recreate_monument(operations, BUILDING_MAUSOLEUM,
                positions->type_de_packed & 0x1fff,
                (int16_t)positions->type_de_first);
        }
        if (positions->type_e1_x != 0 && positions->type_e1_y != 0 &&
            state->settings->mission_id == 51) {
            recreate_monument(operations, BUILDING_LIBRARY,
                (int16_t)positions->type_e1_x, (int16_t)positions->type_e1_y);
        }
        if (positions->type_e4_x != 0 && positions->type_e4_y != 0 &&
            state->settings->mission_id == 51) {
            recreate_monument(operations, BUILDING_LIGHTHOUSE,
                (int16_t)positions->type_e4_x, (int16_t)positions->type_e4_y);
        }
    } else if (options[4] == 'V') {
        if (positions->type_e5_x != 0 && positions->type_e5_y != 0) {
            recreate_monument(operations, BUILDING_SMALL_ROYAL_TOMB,
                (int16_t)positions->type_e5_x, (int16_t)positions->type_e5_y);
        }
        if (positions->type_ea_x != 0 && positions->type_ea_y != 0 &&
            state->settings->mission_id >= 40) {
            recreate_monument(operations, BUILDING_MEDIUM_ROYAL_TOMB,
                (int16_t)positions->type_ea_x, (int16_t)positions->type_ea_y);
        }
        if (positions->type_eb_x != 0 && positions->type_eb_y != 0 &&
            state->settings->mission_id >= 41) {
            recreate_monument(operations, BUILDING_LARGE_ROYAL_TOMB,
                (int16_t)positions->type_eb_x, (int16_t)positions->type_eb_y);
        }
    }

    if (options[2] != '1') {
        for (int32_t kind = 0; kind < 3; ++kind) {
            state->started->legions[kind].experience = -1;
            state->started->legions[kind].soldiers = 0;
            state->started->legions[kind].legion_id = UINT8_MAX;
        }
        *state->tutorial_legions_message_posted = 1;
    } else {
        *state->tutorial_legions_message_posted = 0;
    }
    operations->finalize_records(operations->user_data);
}
